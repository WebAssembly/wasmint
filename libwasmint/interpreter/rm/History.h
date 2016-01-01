/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef WASMINT_HISTORY_H
#define WASMINT_HISTORY_H

#include "MachinePatch.h"
#include <map>

namespace wasmint {

    ExceptionMessage(TargetStateNotInHistory)
    ExceptionMessage(HistoryNotEnabled)

    class History : public HeapObserver {

        class InverseSortedInstructionCounter {
        public:
            InstructionCounter counter;
            InverseSortedInstructionCounter() {
            }
            InverseSortedInstructionCounter(const InstructionCounter& counter) : counter(counter) {
            }
            bool operator<(const InverseSortedInstructionCounter& other) const {
                return counter > other.counter;
            }
        };

        std::map<InverseSortedInstructionCounter, MachinePatch*> patches_;
        std::map<InverseSortedInstructionCounter, uint64_t> nativeFunctionReturnValues_;

        bool enabled_ = false;

    public:
        History() {
        }

        void clear() {
            for (auto& pair : patches_) {
                delete pair.second;
            }
            patches_.clear();
            nativeFunctionReturnValues_.clear();
            enabled_ = false;
        }

        virtual ~History() {
            clear();
        }

        void addCheckpoint(VMState & machine) {
            enabled_ = true;
            patches_[machine.instructionCounter()] = new MachinePatch(machine);
        }

        MachinePatch& getLastCheckpoint() {
            return *patches_.rbegin()->second;
        }

        virtual void preChanged(const Heap& heap, const Interval& changedInterval) override {
            if (enabled_) {
                getLastCheckpoint().preHeapChanged(heap, changedInterval);
            }
        }

        uint64_t getNativeFunctionReturnValue(const InstructionCounter& counter) {
            return nativeFunctionReturnValues_[counter];
        }

        void addNativeFunctionReturnValue(const InstructionCounter& counter, uint64_t value) {
            if (enabled_)
                nativeFunctionReturnValues_[counter] = value;
        }

        void setToState(const InstructionCounter& targetCounter, VMState& state) const {
            if (!enabled_)
                throw HistoryNotEnabled("History recording was not enabled. Can't use setToState()");
            if (targetCounter == state.instructionCounter()) {
                // nothing to do here
                return;
            }
            if (targetCounter < state.instructionCounter()) {
                auto targetIter = patches_.lower_bound(targetCounter);
                if (targetIter == patches_.end()) {
                    throw TargetStateNotInHistory("Can't rollback behind state with counter " + targetCounter.toString());
                } else {
                    auto startIter = patches_.lower_bound(state.instructionCounter());
                    if (startIter == patches_.end()) {
                        throw TargetStateNotInHistory("Can't rollback behind state with counter " + targetCounter.toString());
                    } else {
                        for (;startIter != targetIter; --startIter) {
                            startIter->second->apply(state);
                        }
                        targetIter->second->apply(state);
                    }
                }
            }

            while (state.instructionCounter() < targetCounter) {
                if (!state.step()) {
                    if (state.instructionCounter() != targetCounter) {
                        throw TargetStateNotInHistory("Target state can't be reached (thread has finished)");
                    }
                }
            }
        }

        void threadStackShrinked(VMThread& thread) {
            if (enabled_)
                getLastCheckpoint().preThreadShrinked(thread);
        }
    };
}




#endif //WASMINT_HISTORY_H

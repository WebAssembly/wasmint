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

    class History : public HeapObserver {

        std::map<InstructionCounter, ReverseMachinePatchPtr> patches_;
        std::map<InstructionCounter, uint64_t> nativeFunctionReturnValues_;

    public:
        History() {
        }

        void clear() {
            for (auto& pair : patches_) {
                delete pair.second.ptr;
            }
            patches_.clear();
            nativeFunctionReturnValues_.clear();
        }

        virtual ~History() {
            for (auto& pair : patches_) {
                delete pair.second.ptr;
            }
        }

        void addCheckpoint(VMState & machine) {
            patches_[machine.instructionCounter()] = ReverseMachinePatchPtr(new MachinePatch());
        }

        MachinePatch& getLastCheckpoint() {
            return *patches_.rbegin()->second.ptr;
        }

        virtual void preChanged(const Heap& heap, const Interval& changedInterval) override {
            getLastCheckpoint().preHeapChanged(heap, changedInterval);
        }

        uint64_t getNativeFunctionReturnValue(const InstructionCounter& counter) {
            return nativeFunctionReturnValues_[counter];
        }

        void addNativeFunctionReturnValue(const InstructionCounter& counter, uint64_t value) {
            nativeFunctionReturnValues_[counter] = value;
        }

        void setToState(const InstructionCounter& targetCounter, VMState& state) const {
            if (targetCounter == state.instructionCounter()) {
                // nothing to do here
                return;
            }
            auto targetIter = patches_.lower_bound(targetCounter);
            if (targetIter == patches_.end()) {
                throw TargetStateNotInHistory("Can't rollback behind state with counter " + targetCounter.toString());
            } else {
                auto startIter = patches_.lower_bound(state.instructionCounter());
                if (startIter == patches_.end()) {

                } else {

                }
            }
        }
    };
}




#endif //WASMINT_HISTORY_H

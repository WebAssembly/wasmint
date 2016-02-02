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


#ifndef WASMINT_MACHINEPATCH_H
#define WASMINT_MACHINEPATCH_H


#include <interpreter/heap/patch/HeapPatch.h>
#include <interpreter/heap/HeapObserver.h>
#include "InstructionCounter.h"
#include "VMState.h"
#include "ThreadPatch.h"

namespace wasmint {
    class MachinePatch {

        HeapPatch heapPatch_;
        ThreadPatch threadPatch_;
        InstructionCounter startCounter_;
        bool influencedByExternalState_ = false;

    public:
        MachinePatch() {
        }

        MachinePatch(VMState& machine)
                : heapPatch_(machine.heap()),
                  threadPatch_(machine.thread()),
                  startCounter_(machine.instructionCounter()) {
        }

        void apply(VMState & machine) const {
            heapPatch_.applyPatch(machine.heap());
            threadPatch_.applyPatch(machine.thread());
            machine.instructionCounter(startCounter_);
        }

        void preHeapChanged(const Heap& heap, const Interval& changedInterval) {
            heapPatch_.preHeapChanged(heap, changedInterval);
        }

        const InstructionCounter& startCounter() const {
            return startCounter_;
        }

        void preThreadShrinked(const VMThread& thread) {
            threadPatch_.backupPreShrink(thread);
        }

        void influencedByExternalState(bool value) {
            influencedByExternalState_ = value;
        }

        bool influencedByExternalState() const {
            return influencedByExternalState_;
        }
    };
}



#endif //WASMINT_MACHINEPATCH_H

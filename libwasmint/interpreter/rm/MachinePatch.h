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

        InstructionCounter startCounter_;

        HeapPatch heapPatch_;
        ThreadPatch threadPatch_;

    public:
        MachinePatch() {
        }
        virtual ~MachinePatch() {
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

    };

    class ReverseMachinePatchPtr {
    public:
        MachinePatch* ptr;

        ReverseMachinePatchPtr(MachinePatch* patch = nullptr) : ptr(patch) {
        }
        // Reverse < operator that will sort Machine patches in reverse order
        bool operator<(const ReverseMachinePatchPtr& other) const {
            return other.ptr->startCounter() < ptr->startCounter();
        }
    };
}



#endif //WASMINT_MACHINEPATCH_H

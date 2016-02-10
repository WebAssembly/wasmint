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


#ifndef WASMINT_HALTINGPROBLEMDETECTOR_H
#define WASMINT_HALTINGPROBLEMDETECTOR_H

#include <interpreter/WasmintVM.h>

namespace wasmint {

    ExceptionMessage(CantMakeHaltingDecision)

    class HaltingProblemDetector {

        WasmintVM& vm_;

        bool comparePage(const Heap& a, const Heap& b, std::size_t pageIndex);

        bool isIdentical(const VMState& a, const VMState& b, std::set<std::size_t>& indexes);

    public:
        HaltingProblemDetector(WasmintVM& vm) : vm_(vm) {
        }

        bool isLooping(InstructionCounter startCounter = 0);
    };
}



#endif //WASMINT_HALTINGPROBLEMDETECTOR_H

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


#ifndef WASMINT_REGISTERCOUNTCALCULATOR_H
#define WASMINT_REGISTERCOUNTCALCULATOR_H


#include <cstdint>
#include <instructions/Instruction.h>

namespace wasmint {
    class RegisterAllocator {

    public:
        static std::size_t calculateNumberOfRegisters(const wasm_module::Instruction& instruction, std::size_t parent = 1);
    };
}



#endif //WASMINT_REGISTERCOUNTCALCULATOR_H

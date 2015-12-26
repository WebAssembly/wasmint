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
#include <map>

namespace wasmint {
    class RegisterAllocator {

        std::map<const wasm_module::Instruction*, std::size_t> registers_;
        std::size_t registersRequired_ = 0;

        void setRegister(const wasm_module::Instruction *instruction, std::size_t registerIndex) {
            registers_[instruction] = registerIndex;
            std::size_t newRegistersRequired = registerIndex + 1;
            if (newRegistersRequired > registersRequired_)
                registersRequired_ = newRegistersRequired;
        }

    public:
        void allocateRegisters(const wasm_module::Instruction* instruction, std::size_t offset = 0);

        uint16_t operator()(const wasm_module::Instruction* instr) {
            auto iter = registers_.find(instr);
            if (iter != registers_.end()) {
                return (uint16_t) iter->second;
            } else {
                throw std::domain_error("Instruction " + instr->name() + " isn't handled by this register allocator");
            }
        }

        uint16_t registersRequired() {
            return (uint16_t) registersRequired_;
        }
    };
}



#endif //WASMINT_REGISTERCOUNTCALCULATOR_H

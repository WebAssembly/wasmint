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


#ifndef WASMINT_JITCOMPILER_H
#define WASMINT_JITCOMPILER_H


#include "RegisterAllocator.h"
#include "ByteCode.h"
#include <Function.h>

namespace wasmint {
    class JITCompiler {

        RegisterAllocator registerAllocator_;
        ByteCode code_;
        std::map<const wasm_module::Instruction*, uint32_t> instructionStartAddresses;
        std::map<const wasm_module::Instruction*, uint32_t> instructionEndAddresses;

        std::vector<std::pair<const wasm_module::Instruction*, uint32_t>> needsInstructionStartAddress;
        std::vector<std::pair<const wasm_module::Instruction*, uint32_t>> needsInstructionEndAddress;


        void compileInstruction(const wasm_module::Instruction* instruction);

        void addBranchAddress(const wasm_module::Instruction* instruction, bool before);
        void addBranch(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before);
        void addBranch(const wasm_module::BranchInformation* information, uint16_t opcodeData);
        void addBranchIf(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before);
        void addCopyRegister(uint16_t target, uint16_t source);

    public:
        JITCompiler() {
        }

        void compile(const wasm_module::Function* function) {
            registerAllocator_ = RegisterAllocator();
            registerAllocator_.allocateRegisters(function->mainInstruction());
        }

        const ByteCode& code() const {
            return code_;
        }

    };
}



#endif //WASMINT_JITCOMPILER_H

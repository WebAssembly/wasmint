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
#include <stdexcept>

namespace wasmint {
    class WasmintVM;

    class JITCompiler {

        RegisterAllocator registerAllocator_;
        ByteCode code_;
        std::map<const wasm_module::Instruction*, uint32_t> instructionStartAddresses;
        std::map<const wasm_module::Instruction*, uint32_t> instructionEndAddresses;

        std::map<uint32_t, const wasm_module::Instruction*> instructionFinishedAddresses;

        std::vector<std::pair<const wasm_module::Instruction*, uint32_t>> needsInstructionStartAddress;
        std::vector<std::pair<const wasm_module::Instruction*, uint32_t>> needsInstructionEndAddress;
        std::vector<std::pair<wasm_module::FunctionSignature, uint32_t>> needsFunctionIndex;

        void compileInstruction(const wasm_module::Instruction* instruction);

        void addBranchAddress(const wasm_module::Instruction* instruction, bool before);
        void addBranchAddress(const wasm_module::BranchInformation* information);
        void addBranch(const wasm_module::Instruction* instruction, bool before);
        void addBranch(const wasm_module::BranchInformation* information);
        void addBranchIf(const wasm_module::BranchInformation* information, uint16_t opcodeData);
        void addBranchIfNot(const wasm_module::BranchInformation* information, uint16_t opcodeData);
        void addBranchIf(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before);
        void addBranchIfNot(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before);
        void addBranchCopyReg(const wasm_module::BranchInformation* information, uint16_t targetReg, uint16_t srcReg);
        void addBranchCopyReg(const wasm_module::Instruction* instruction, uint16_t targetReg, uint16_t srcReg);
        void addBranchIfCopyReg(const wasm_module::BranchInformation* information, uint16_t opcodeData, uint16_t targetReg, uint16_t srcReg);
        void addCopyRegister(uint16_t target, uint16_t source);

        void linkLocally();

    public:
        JITCompiler() {
        }

        void compile(const wasm_module::Function* function);

        const ByteCode& code() const {
            return code_;
        }

        void linkGlobally(const WasmintVM* registerMachine);

        const wasm_module::Instruction* getInstruction(uint32_t address) const {
            auto iter = instructionFinishedAddresses.find(address);
            if (iter != instructionFinishedAddresses.end()) {
                return iter->second;
            } else {
                return nullptr;
            }
        }

    };
}



#endif //WASMINT_JITCOMPILER_H

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


#ifndef WASMINT_COMPILEDFUNCTION_H
#define WASMINT_COMPILEDFUNCTION_H

#include <Function.h>
#include <interpreter/debugging/Breakpoint.h>
#include "ByteCode.h"
#include "JITCompiler.h"

namespace wasmint {
    class VMState;

    class CompiledFunction {
        const wasm_module::Function* function_;
        JITCompiler debugCompiler_;
        std::unordered_map<uint32_t, Breakpoint> breakpointsByInstructionAddress_;

    public:
        CompiledFunction() {
        }
        CompiledFunction(const wasm_module::Function* function) : function_(function) {
            debugCompiler_.compile(function);
        }

        const ByteCode& code() const {
            return debugCompiler_.code();
        }

        const wasm_module::Function& function() const {
            return *function_;
        }

        const JITCompiler& jitCompiler() const {
            return debugCompiler_;
        }

        JITCompiler& jitCompiler() {
            return debugCompiler_;
        }

        void addBreakpoint(const wasm_module::Instruction* instruction, BreakpointHandler* handler = nullptr) {
            uint32_t address = debugCompiler_.getInstructionEndAddress(instruction);
            breakpointsByInstructionAddress_[address] = Breakpoint(instruction, handler);
        }

        bool triggerBreakpoints(VMState& runner, uint32_t instructionPointer) {
            auto iter = breakpointsByInstructionAddress_.find(instructionPointer);
            if (iter != breakpointsByInstructionAddress_.end()) {
                iter->second.trigger(runner);
                return true;
            } else {
                return false;
            }
        }
    };

}


#endif //WASMINT_COMPILEDFUNCTION_H

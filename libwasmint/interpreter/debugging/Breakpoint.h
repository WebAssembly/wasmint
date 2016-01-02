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


#ifndef WASMINT_BREAKPOINT_H
#define WASMINT_BREAKPOINT_H

#include <instructions/Instruction.h>
#include <functional>

namespace wasmint {

    class BreakpointHandler;
    class VMState;

    class Breakpoint {

        const wasm_module::Instruction *instruction_ = nullptr;
        BreakpointHandler* handler_ = nullptr;

    public:
        Breakpoint() {
        }

        Breakpoint(const wasm_module::Instruction *instruction, BreakpointHandler* handler = nullptr)
                : instruction_(instruction), handler_(handler) {
        }

        const wasm_module::Instruction *instruction() const {
            return instruction_;
        }

        BreakpointHandler* handler() {
            return handler_;
        }

        void trigger(VMState& state);

    };

}

#endif //WASMINT_BREAKPOINT_H

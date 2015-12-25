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


#ifndef WASMINT_REGISTERMACHINE_H
#define WASMINT_REGISTERMACHINE_H

#include <interpreter/heap/Heap.h>
#include <Function.h>
#include "ByteCode.h"
#include "JITCompiler.h"
#include "ByteCodeRunner.h"
#include "CompiledFunction.h"

namespace wasmint {
    class RegisterMachine {

        Heap heap_;
        ByteCodeRunner runner_;
        std::vector<CompiledFunction*> functions_;

    public:
        RegisterMachine() : runner_(this) {
        }

        void startAtFunction(wasm_module::Function& function) {
            for (std::size_t i = 0; i < functions_.size(); i++) {
                if (&functions_[i]->function() == &function) {
                    break;
                }
            }
        }

        CompiledFunction& getCompiledFunction(std::size_t index) {
            return *functions_.at(index);
        }

        std::size_t getIndex(const CompiledFunction& function) {
            for (std::size_t i = 0; i < functions_.size(); i++) {
                if (functions_[i] == &function) {
                    return i;
                }
            }
            throw std::domain_error("Function " + function.function().name() + " not compiled in this RegisterMachine");
        }

    };
};




#endif //WASMINT_REGISTERMACHINE_H

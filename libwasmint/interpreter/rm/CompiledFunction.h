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
#include "ByteCode.h"
#include "JITCompiler.h"

namespace wasmint {
    class CompiledFunction {
        wasm_module::Function* function_;
        JITCompiler compiler_;

    public:
        CompiledFunction() {
        }
        CompiledFunction(wasm_module::Function* function) : function_(function) {
            compiler_.compile(function);
        }

        const ByteCode& code() const {
            return compiler_.code();
        }

        const wasm_module::Function& function() const {
            return *function_;
        }
    };

}


#endif //WASMINT_COMPILEDFUNCTION_H

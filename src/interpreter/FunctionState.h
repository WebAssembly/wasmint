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

#ifndef WASMINT_FUNCTIONSTATE_H
#define WASMINT_FUNCTIONSTATE_H


#include <Variable.h>


namespace wasm_module {
    class Function;
}

namespace wasmint {

    class FunctionState {

        std::vector<wasm_module::Variable> variables_;

    public:
        FunctionState() {
        }

        FunctionState(wasm_module::Function &function);

        wasm_module::Variable &variable(uint32_t index) {
            return variables_.at(index);
        }
    };

}

#endif //WASMINT_FUNCTIONSTATE_H

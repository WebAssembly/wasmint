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

#include "FunctionState.h"

#include <Function.h>

namespace wasmint {

    FunctionState::FunctionState(wasm_module::Function &function) : module_(&function.module()) {
        for (const wasm_module::Type* type : function.locals()) {
            variables_.push_back(wasm_module::Variable(type));
        }
    }

    FunctionState::FunctionState(wasm_module::Function &function, std::vector<const wasm_module::Type*>& variadicParameters) : module_(&function.module()) {
        for (const wasm_module::Type* type : variadicParameters) {
            variables_.push_back(wasm_module::Variable(type));
        }
    }

    void FunctionState::setState(ByteInputStream& stream) {
        uint64_t numberOfVariables = stream.getUInt64();
        for (uint64_t i = 0; i < numberOfVariables; i++) {
            variables_.push_back(stream.getVariable());
        }
    }

    void FunctionState::serialize(ByteOutputStream& stream) const {
        stream.writeUInt64(variables_.size());
        for (std::size_t i = 0; i < variables_.size(); i++) {
            stream.writeVariable(variables_[i]);
        }
    }
}
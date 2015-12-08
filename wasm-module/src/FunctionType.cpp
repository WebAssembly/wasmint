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

#include <types/Void.h>
#include "FunctionType.h"

std::string wasm_module::FunctionType::toString() const {
    std::string result;
    result += "((result";
    if (returnType() != Void::instance()) {
        result += returnType()->name();
    }
    result += ") ";

    for (const Type* type : parameters()) {
        result += "(param ";
        result += type->name();
        result += ") ";
    }
    result.resize(result.size() - 1);
    result += ")";
    return result;
}

wasm_module::FunctionType::FunctionType() {
    returnType_ = Void::instance();
}
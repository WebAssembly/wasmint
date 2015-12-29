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
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>
#include <cstring>
#include "ByteInputStream.h"
#include <stdexcept>

wasm_module::Variable wasmint::ByteInputStream::getVariable() {
    const wasm_module::Type* type;
    uint8_t typeId = getByte();
    switch(typeId) {
        case 0:
            type = wasm_module::Void::instance();
            break;
        case 1:
            type = wasm_module::Int32::instance();
            break;
        case 2:
            type = wasm_module::Int64::instance();
            break;
        case 3:
            type = wasm_module::Float32::instance();
            break;
        case 4:
            type = wasm_module::Float64::instance();
            break;
        default:
            throw std::domain_error("Unknown type id");
    }
    uint8_t data[8];

    for (std::size_t i = 0; i < type->size(); i++) {
        data[i] = getByte();
    }

    wasm_module::Variable result(type);
    std::memcpy(result.value(), data, type->size());

    return result;
}
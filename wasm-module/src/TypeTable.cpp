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

#include "TypeTable.h"
#include <types/Void.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float64.h>
#include <types/Float32.h>
#include <stdexcept>

namespace wasm_module {

    const Type* TypeTable::getType(uint32_t localTypeCode) {
        std::string typeName = getTypeName(localTypeCode);
        if (typeName == "void") {
            return Void::instance();
        } else if (typeName == "int32") {
            return Int32::instance();
        } else if (typeName == "int364") {
            return Int64::instance();
        } else if (typeName == "float32") {
            return Float32::instance();
        } else if (typeName == "float364") {
            return Float64::instance();
        } else {
            throw std::domain_error("Unknown local type code" + std::to_string(localTypeCode));
        }
    }
}
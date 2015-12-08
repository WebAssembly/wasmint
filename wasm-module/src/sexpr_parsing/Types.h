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

#ifndef WASMINT_TYPES_H
#define WASMINT_TYPES_H


#include <types/Type.h>
#include <unordered_map>
#include <ExceptionWithMessage.h>
#include <types/Int32.h>
#include <types/Float32.h>
#include <types/Int64.h>
#include <types/Float64.h>
#include <types/Void.h>

namespace wasm_module { namespace sexpr {

    ExceptionMessage(UnknownTypeName);

    class Types {

        std::unordered_map<std::string, const Type*> typesByName_;

        Types() {
            typesByName_ = {
                    {"i32", Int32::instance()},
                    {"f32", Float32::instance()},
                    {"i64", Int64::instance()},
                    {"f64", Float64::instance()},
                    {"void", Void::instance()}
            };
        }

    public:
        static const Type* getByName(const std::string& typeName) {
            static Types types;

            auto iter = types.typesByName_.find(typeName);

            if (iter != types.typesByName_.end()) {
                return iter->second;
            } else {
                throw UnknownTypeName(typeName);
            }
        }


    };
}}


#endif //WASMINT_TYPES_H

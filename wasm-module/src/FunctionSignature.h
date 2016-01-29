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

#ifndef WASMINT_FUNCTIONSIGNATURE_H
#define WASMINT_FUNCTIONSIGNATURE_H

#include <string>
#include <vector>
#include <types/Void.h>
#include <stdexcept>

#include "types/Type.h"
#include "Variable.h"
#include "FunctionType.h"

namespace wasm_module {

    /**
     * Contains all information that are relevant to an caller of the given function.
     */
    class FunctionSignature : public FunctionType {

        std::string moduleName_;
        std::string name_;

    public:
        FunctionSignature() {
        }

        FunctionSignature(std::string module, std::string name, const Type* returnType,
                          std::vector<const Type*> parameterTypes)
                : FunctionType(returnType, parameterTypes), moduleName_(module), name_(name) {
        }

        FunctionSignature(std::string module, std::string name, const Type* returnType)
                : FunctionType(returnType), moduleName_(module), name_(name) {
        }

        const std::string& name() const {
            return name_;
        }

        const std::string& moduleName() const {
            return moduleName_;
        }

        bool operator<(const FunctionSignature& other) const {
            if (name_ < other.name())
                return true;
            if (returnType() < other.returnType())
                return true;
            return false;
        }

        bool operator==(const FunctionSignature& other) const {
            return name_ == other.name_
            && moduleName_ == other.moduleName_
            && other.FunctionType::operator==(other);
        }
    };

}

#endif //WASMINT_FUNCTIONSIGNATURE_H

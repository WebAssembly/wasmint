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

#ifndef WASMINT_FUNCTIONCONTEXT_H
#define WASMINT_FUNCTIONCONTEXT_H


#include "types/Type.h"
#include "FunctionSignature.h"
#include "ExceptionWithMessage.h"
#include <vector>
#include <unordered_map>
#include <map>

namespace wasm_module {

    ExceptionMessage(UnknownVariableName);
    ExceptionMessage(UnknownVariableIndex);

    /**
     * The context of a function. This contains all information that are needed to
     * create the instructions in the AST of the related function.
     */
    class FunctionContext : public FunctionSignature {

        std::vector<const Type*> locals_;
        std::unordered_map<std::string, uint32_t> namesToIndizes_;
        std::unordered_map<uint32_t, std::string> indizesToNames_;

    public:
        FunctionContext() {
        }

        FunctionContext(std::string module, std::string name, const Type* returnType, std::vector<const Type*> parameterTypes,
                        std::vector<const Type*> locals)
                : locals_(locals), FunctionSignature(module, name, returnType, parameterTypes) {
        }

        FunctionContext(std::string module, std::string name, const Type* returnType, std::vector<const Type*> locals)
                : locals_(locals), FunctionSignature(module, name, returnType) {
        }

        const std::vector<const Type*>& pureLocals() const {
            return locals_;
        }

        std::vector<const Type*> locals() const {
            std::vector<const Type*> result;

            for (const Type* type : parameters()) {
                result.push_back(type);
            }

            for (const Type* type : pureLocals()) {
                result.push_back(type);
            }
            return result;
        }

        void setVariableNameToIndexMap(const std::unordered_map<std::string, uint32_t> map) {
            namesToIndizes_ = map;
            for (auto& pair : map) {
                indizesToNames_[pair.second] = pair.first;
            }
        }

        uint32_t variableNameToIndex(const std::string& name) const {
            auto iter = namesToIndizes_.find(name);
            if (iter != namesToIndizes_.end()) {
                return iter->second;
            } else {
                throw UnknownVariableName(name);
            }
        }

        std::string variableName(uint32_t variableIndex) const {
            auto iter = indizesToNames_.find(variableIndex);
            if (iter != indizesToNames_.end()) {
                return iter->second;
            } else {
                throw UnknownVariableIndex(std::to_string(variableIndex));
            }
        }
    };

}

#endif //WASMINT_FUNCTIONCONTEXT_H

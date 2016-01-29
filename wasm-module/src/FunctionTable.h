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

#ifndef WASMINT_FUNCTIONTABLE_H
#define WASMINT_FUNCTIONTABLE_H

#include <cstdint>
#include <string>
#include <map>
#include "FunctionSignature.h"
#include "ExceptionWithMessage.h"
#include "Utils.h"

namespace wasm_module {

    ExceptionMessage(UnknownLocalFunctionId)
    ExceptionMessage(UnknownFunctionName)

    /**
     * Maps local function ids to function signatures.
     */
    class FunctionTable {

        std::vector<FunctionSignature> functions_;
        std::map<std::string, std::size_t> functionsByAlias_;

    public:
        FunctionTable() {
        }

        void addFunctionSignature(FunctionSignature signature, std::string alias) {
            functionsByAlias_[alias] = functions_.size();
            functions_.push_back(signature);
        }

        void addFunctionSignature(FunctionSignature signature) {
            functions_.push_back(signature);
        }

        bool hasFunctionSignature(const std::string& alias) const {
            auto iter = functionsByAlias_.find(alias);
            return iter != functionsByAlias_.end();
        }

        const FunctionSignature& getFunctionSignature(const std::string& alias) const {
            auto iter = functionsByAlias_.find(alias);
            if (iter != functionsByAlias_.end()) {
                return getFunctionSignature(iter->second);
            } else {
                throw UnknownFunctionName(alias);
            }
        }

        const FunctionSignature& getFunctionSignature(std::size_t localFunctionId) const {
            if (localFunctionId >= functions_.size())
                throw UnknownLocalFunctionId(std::to_string(localFunctionId));
            return functions_.at(localFunctionId);
        }

        std::size_t size() const {
            return functions_.size();
        }

        bool operator==(const FunctionTable& other) const {
            return  Utils::compareVector(functions_, other.functions_) &&
                    Utils::compareMaps(functionsByAlias_, other.functionsByAlias_);
        }
    };

}

#endif //WASMINT_FUNCTIONTABLE_H

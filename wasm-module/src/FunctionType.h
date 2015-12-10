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

#ifndef WASMINT_FUNCTIONTYPE_H
#define WASMINT_FUNCTIONTYPE_H

#include <types/Type.h>
#include <vector>
#include <stdexcept>

namespace wasm_module {

    class FunctionType {
        const Type* returnType_;
        std::vector<const Type*> parameters_;
        bool variadic_ = true;
        std::size_t index_ = 0;
        bool hasIndex_ = false;

    public:
        FunctionType();
        FunctionType(const Type* returnType, const std::vector<const Type*>& parameters) : returnType_(returnType), parameters_(parameters), variadic_(false) {
        }
        FunctionType(const Type* returnType) : returnType_(returnType), variadic_(true) {
        }

        const std::vector<const Type*>& parameters() const {
            return parameters_;
        }

        const Type* returnType() const {
            return returnType_;
        }

        bool variadic() const {
            return variadic_;
        }

        bool compatibleWith(const FunctionType& other) const {
            if (returnType_ != other.returnType_)
                return false;

            if (variadic() || other.variadic())
                return true;

            if (parameters_.size() != other.parameters().size())
                return false;

            for (std::size_t i = 0; i < other.parameters().size(); i++) {
                if (parameters()[i] != other.parameters()[i]) {
                    return false;
                }
            }

            if (hasIndex() && other.hasIndex()) {
                if (this->index() != other.index()) {
                    return false;
                }
            }

            return true;
        }

        bool hasIndex() const {
            return hasIndex_;
        }

        std::size_t index() const {
            if (!hasIndex_)
                throw std::domain_error("Can't call index() on FunctionType instance that has no index");
            return index_;
        }

        void index(std::size_t newIndex) {
            index_ = newIndex;
            hasIndex_ = true;
        }

        std::string toString() const;
    };

}


#endif //WASMINT_FUNCTIONTYPE_H

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

#include <iostream>
#include "SExpr.h"

namespace wasm_module { namespace sexpr {

        std::string SExpr::toString(unsigned int intend) const {
            if (hasValue()) {
                if (value_.empty())
                    return "\"\"";
                return value_;
            } else {
                std::stringstream result;
                result << '\n' << std::string(intend, ' ');
                result << "(";

                bool hasNonValueChildren = false;

                std::size_t i = 0;
                for (const SExpr& child : children_) {
                    result << child.toString(intend + 4u);
                    if (i != (children_.size() - 1u))
                        result << " ";
                    if (child.hasChildren())
                        hasNonValueChildren = true;
                    i++;
                }

                if (hasNonValueChildren)
                    result << '\n' << std::string(intend, ' ');
                result << ")";
                return result.str();
            }
        }

        SExpr &SExpr::operator[](std::size_t i) {
            if (!hasChildren())
                throw SExprHasNoChildren(toString());
            if (i >= children_.size()) {
                throw SExprChildrenRangeError("Tried accessing child with out of bounds index " + std::to_string(i) + ". Expression is: " + toString());
            }

            return children_.at(i);
        }

        const SExpr &SExpr::operator[](std::size_t i) const {
            if (!hasChildren())
                throw SExprHasNoChildren(toString());
            if (i >= children_.size()) {
                throw SExprChildrenRangeError("Tried accessing child with out of bounds index " + std::to_string(i) + " " + toString());
            }
            return children().at(i);
        }
    }}
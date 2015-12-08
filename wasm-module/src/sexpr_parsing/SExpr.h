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

#ifndef WASMINT_SEXPR_H
#define WASMINT_SEXPR_H

#include <vector>
#include <string>
#include <sstream>
#include <ExceptionWithMessage.h>

namespace wasm_module { namespace sexpr {

    class SExprIsFull : public std::exception {
    };

    ExceptionMessage(SExprHasNoValue)
    ExceptionMessage(SExprHasNoChildren)
    ExceptionMessage(SExprChildrenRangeError)

    class SExpr {

        bool hasValue_ = false;
        std::string value_;

        std::vector<SExpr> children_;

    public:
        SExpr() {
        }

        SExpr(std::string value) : value_(value), hasValue_(true) {
        }

        SExpr &addChild() {
            if (hasValue())
                throw SExprIsFull();

            children_.push_back(SExpr());
            return lastChild();
        }

        SExpr& addChild(const std::string& value) {
            if (hasValue())
                throw SExprIsFull();

            children_.push_back(SExpr(value));
            return lastChild();
        }

        SExpr& addChild(const SExpr& expr) {
            if (hasValue())
                throw SExprIsFull();

            children_.push_back(expr);
            return lastChild();
        }

        SExpr &lastChild() {
            if (!hasChildren())
                throw SExprHasNoChildren(toString());
            return children_.back();
        }

        bool hasChildren() const {
            return !hasValue();
        }

        bool hasValue() const {
            return hasValue_;
        }

        void insertChild(const SExpr& child, std::size_t pos) {
            if (hasValue())
                throw SExprHasNoChildren(toString());
            children_.insert(children_.begin() + pos, child);
        }

        void removeChild(std::size_t pos) {
            children_.erase(children_.begin() + pos);
        }

        const std::string &value() const {
            if (!hasValue()) {
                throw SExprHasNoValue(toString());
            }
            return value_;
        }

        const std::vector<SExpr> &children() const {
            if (!hasChildren())
                throw SExprHasNoChildren(toString());
            return children_;
        }

        const SExpr& operator[](std::size_t i) const;

        SExpr& operator[](std::size_t i);

        bool operator!=(const std::string& otherValue) const {
            return value() != otherValue;
        }

        bool operator==(const std::string& otherValue) const {
            return value() == otherValue;
        }

        std::string toString(unsigned int intend = 0) const;

    };
}}

#endif //WASMINT_SEXPR_H

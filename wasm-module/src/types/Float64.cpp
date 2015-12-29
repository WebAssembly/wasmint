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

#include <limits>
#include "Float64.h"
#include "Int64.h"
#include <stdexcept>

namespace wasm_module {

    bool Float64::tryParseNan(const std::string& literal, void *data) const {
        if (literal == "nan" || literal == "+nan") {
            (*(uint64_t*) data) = 0x7ff8000000000000ul;
            return true;
        }
        if (literal == "-nan") {
            (*(uint64_t*) data) = 0xfff8000000000000ul;
            return true;
        }

        bool negative = false;

        std::string value = literal;
        if (literal.substr(0, 4) == "nan:") {
            value = value.substr(4);
        } else if (literal.substr(0, 5) == "+nan:") {
            value = value.substr(5);
        } else if (literal.substr(0, 5) == "-nan:") {
            value = value.substr(5);
            negative = true;
        } else {
            return false;
        }

        Int64::instance()->parse(value, data);
        if (negative)
            (*(uint64_t*) data) |= 0xfff0000000000000ul;
        else
            (*(uint64_t*) data) |= 0x7ff0000000000000ul;
        return true;
    }

    void Float64::parse(const std::string& literal, void *data) const {
        if (tryParseNan(literal, data)) {
            // nothing to do
        } else {
            const char* literalC = literal.c_str();
            char* outPtr = const_cast<char*>(literalC);
            (*(double*) data) = std::strtod(literalC, &outPtr);

            if (outPtr == literalC) {
                throw std::domain_error("Couldn't parse " + literal + " as float32");
            }
        }
    }
}
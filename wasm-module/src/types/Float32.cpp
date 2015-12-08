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
#include "Float32.h"
#include "Int32.h"
#include <stdexcept>

namespace wasm_module {

    bool Float32::tryParseNan(const std::string& literal, void *data) const {
        if (literal == "nan" || literal == "+nan") {
            (*(uint32_t*) data) = 0x7fc00000u;
            return true;
        }
        if (literal == "-nan") {
            (*(uint32_t*) data) = 0xffc00000;
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

        Int32::instance()->parse(value, data);
        if (negative)
            (*(uint32_t*) data) |= 0xff800000u;
        else
            (*(uint32_t*) data) |= 0x7f800000u;
        return true;
    }

    void Float32::parse(const std::string& literal, void *data) const {
        if (tryParseNan(literal, data)) {
            // nothing to do
        } else {
            const char* literalC = literal.c_str();
            char* outPtr = const_cast<char*>(literalC);
            (*(float*) data) = std::strtof(literalC, &outPtr);

            if (outPtr == literalC) {
                throw std::domain_error("Couldn't parse " + literal + " as float32");
            }
        }
    }
}
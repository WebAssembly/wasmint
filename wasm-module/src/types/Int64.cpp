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



#include <Utils.h>
#include "Int64.h"

namespace wasm_module {

    void Int64::parse(const std::string& literal, void *data) const {
        if (Utils::isHexNumber(literal, 16)) {
            if (literal.at(0) == '-') {
                int64_t value = 0;
                for(std::size_t i = 3; i < literal.size(); i++) {
                    value *= 16;
                    char c = literal[i];
                    if (c >= 'a' && c <= 'f') {
                        value -= c - 'a' + 10u;
                    } else if (c >= 'A' && c <= 'F') {
                        value -= c - 'A' + 10u;
                    } else {
                        value -= c - '0';
                    }
                }
                (*(int64_t*) data) = value;
            } else {
                uint64_t value = 0;
                for(std::size_t i = 2; i < literal.size(); i++) {
                    value <<= 4u;
                    char c = literal[i];
                    if (c >= 'a' && c <= 'f') {
                        value |= c - 'a' + 10u;
                    } else if (c >= 'A' && c <= 'F') {
                        value |= c - 'A' + 10u;
                    } else {
                        value |= c - '0';
                    }
                }
                (*(uint64_t*) data) = value;
            }
        } else if (Utils::isDecNumber(literal, true)) {
            if (literal.at(0) == '-') {
                int64_t value = 0;
                for(std::size_t i = 0; i < literal.size(); i++) {
                    char c = literal[i];
                    if (c == '-')
                        continue;

                    value *= 10;
                    value -= c - '0';
                }
                (*(int64_t*) data) = value;
            } else {
                uint64_t value = 0;
                for(std::size_t i = 0; i < literal.size(); i++) {
                    char c = literal[i];
                    value *= 10;
                    value += c - '0';
                }
                (*(uint64_t*) data) = value;
            }
        } else {
            throw InvalidI64Format(literal);
        };

    }
}

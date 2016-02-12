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


#ifndef WASMINT_STRINGCHARACTERSTREAM_H
#define WASMINT_STRINGCHARACTERSTREAM_H

#include <string>
#include <iostream>
#include "CharacterStream.h"

namespace wasm_module { namespace sexpr {

        class StringCharacterStream : public CharacterStream {

            std::string value_;
            std::size_t position = 0;

        public:
            StringCharacterStream() {
            }

            StringCharacterStream(const std::string& value) : value_(value) {
            }

            virtual char peekChar() const override {
                if (position >= value_.size()) {
                    throw UnexpectedEndOfCharacterStream();
                }
                return value_[position];
            }

            virtual char popChar() override {
                char result = peekChar();
                if (nextIsNewLine_) {
                    nextIsNewLine_ = false;
                    line_++;
                    linePos_ = 0;
                }
                linePos_++;
                if (result == '\n') {
                    nextIsNewLine_ = true;
                }
                position++;
                return result;
            }

            virtual bool reachedEnd() const override {
                return position >= value_.size();
            }
        };

    }}


#endif //WASMINT_STRINGCHARACTERSTREAM_H

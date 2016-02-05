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

#ifndef WASMINT_CHARACTERSTREAM_H
#define WASMINT_CHARACTERSTREAM_H

#include <string>

namespace wasm_module { namespace sexpr {

    class UnexpectedEndOfCharacterStream : public std::exception {
    };

    class CharacterStream {

        std::string value_;
        std::size_t position = 0;

        std::size_t line_ = 1;
        std::size_t linePos_ = 1;
        bool nextIsNewLine_ = false;


    public:
        CharacterStream() {
        }

        CharacterStream(const std::string& value) : value_(value) {
            trimWhitespace();
        }

        char peekChar() const {
            if (position >= value_.size()) {
                throw UnexpectedEndOfCharacterStream();
            }
            return value_[position];
        }

        char popChar() {
            if (nextIsNewLine_) {
                nextIsNewLine_ = false;
                line_++;
                linePos_ = 0;
            }
            char result = peekChar();
            linePos_++;
            if (result == '\n') {
                nextIsNewLine_ = true;
            }
            position++;
            return result;
        }

        bool isWhitespace(char c) {
            return c == ' ' || c == '\n' || c == '\t';
        }

        std::string popWord() {
            std::string result;
            trimWhitespace();
            char c;
            while (!reachedEnd() && !isWhitespace(c = popChar())) {
                result.push_back(c);
            }
            trimWhitespace();
            return result;
        }

        void trimWhitespace() {
            while (!reachedEnd() && isWhitespace(peekChar())) {
                popChar();
            }
        }

        bool reachedEnd() const {
            return position >= value_.size();
        }

        std::size_t line() const {
            return line_;
        }

        std::size_t linePos() const {
            return linePos_;
        }
    };

}}

#endif //WASMINT_CHARACTERSTREAM_H

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


#ifndef WASMINT_FILECHARACTERSTREAM_H
#define WASMINT_FILECHARACTERSTREAM_H

#include <string>
#include <fstream>
#include "CharacterStream.h"

namespace wasm_module { namespace sexpr {
    class FileCharacterStream : public CharacterStream {
        std::ifstream inputStream;
        char nextCharacter_;
        bool hasNextChar_;

        void getNextChar() {
            hasNextChar_ = static_cast<bool>(inputStream.get(nextCharacter_));
        }

    public:
        FileCharacterStream(const std::string& path) : inputStream(path) {
            getNextChar();
        }
        virtual ~FileCharacterStream() {
            inputStream.close();
        }

        virtual char peekChar() const override {
            if (!hasNextChar_) {
                throw UnexpectedEndOfCharacterStream();
            }
            return nextCharacter_;
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
            getNextChar();
            return result;
        }

        virtual bool reachedEnd() const {
            return !hasNextChar_;
        }
    };
}}


#endif //WASMINT_FILECHARACTERSTREAM_H

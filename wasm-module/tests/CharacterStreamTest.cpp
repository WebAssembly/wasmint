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

#include <cstdint>
#include <binary_parsing/ByteStream.h>
#include <Module.h>
#include <binary_parsing/ModuleParser.h>
#include <cassert>
#include <types/Int32.h>
#include <sexpr_parsing/CharacterStream.h>
#include <sexpr_parsing/StringCharacterStream.h>

using namespace wasm_module::sexpr;

int main() {
    std::string str1 = "these are four words";

    {

        StringCharacterStream stream(str1);
        stream.trimWhitespace();
        stream.trimWhitespace();

        assert(!stream.reachedEnd());
        assert(stream.peekChar() == 't');
        assert(stream.popWord() == "these");
        assert(!stream.reachedEnd());
        stream.trimWhitespace();

        assert(stream.peekChar() == 'a');
        assert(stream.popWord() == "are");
        assert(!stream.reachedEnd());
        stream.trimWhitespace();

        assert(stream.peekChar() == 'f');
        assert(stream.popWord() == "four");
        assert(!stream.reachedEnd());
        stream.trimWhitespace();

        assert(stream.peekChar() == 'w');
        assert(stream.popWord() == "words");
        assert(stream.reachedEnd());
        try {
            stream.popChar();
            assert(false);
        } catch (const UnexpectedEndOfCharacterStream &ex) {
        }
    }

    {
        StringCharacterStream stream("");
        assert(stream.reachedEnd());
        try {
            stream.popChar();
            assert(false);
        } catch (const UnexpectedEndOfCharacterStream &ex) {
        }
    }
}
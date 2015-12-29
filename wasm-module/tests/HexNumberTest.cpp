/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License"));
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
#include <sexpr_parsing/SExprParser.h>
#include <Utils.h>

using namespace wasm_module;
using namespace wasm_module::sexpr;

int main() {
    assert(Utils::isHexNumber("0x0", 16));
    assert(Utils::isHexNumber("-0x0", 16));
    assert(Utils::isHexNumber("0X0", 16));
    assert(Utils::isHexNumber("-0X0", 16));
    assert(Utils::isHexNumber("0xabcdefABCDEF0123456789", 32));
    assert(Utils::isHexNumber("-0xabcdefABCDEF0123456789", 32));
    assert(Utils::isHexNumber("0XabcdefABCDEF0123456789", 32));
    assert(Utils::isHexNumber("-0XabcdefABCDEF0123456789", 32));


    assert(!Utils::isHexNumber("--0x0", 16));
    assert(!Utils::isHexNumber("-0xghijklmnopqrstuvwxzy", 16));
    assert(!Utils::isHexNumber("-0xag", 16));
    assert(!Utils::isHexNumber("-0x0g", 16));
    assert(!Utils::isHexNumber(" -0xg", 16));
    assert(!Utils::isHexNumber("-0x", 16));
    assert(!Utils::isHexNumber("a", 16));
    assert(!Utils::isHexNumber("-a", 16));

    assert(!Utils::isHexNumber(" -0xghijklmnopqrstuvwxzy", 16));

    assert(Utils::isHexNumber("0x0", 1));
    assert(Utils::isHexNumber("0x01", 2));
    assert(Utils::isHexNumber("0xab", 2));
    assert(Utils::isHexNumber("0xabc", 3));
    assert(Utils::isHexNumber("-0x0", 1));
    assert(Utils::isHexNumber("-0x01", 2));
    assert(Utils::isHexNumber("-0xab", 2));
    assert(Utils::isHexNumber("-0xabc", 3));

    assert(!Utils::isHexNumber("0x0", 0));
    assert(!Utils::isHexNumber("0x01", 1));
    assert(!Utils::isHexNumber("0xab", 1));
    assert(!Utils::isHexNumber("0xabc", 2));
    assert(!Utils::isHexNumber("0xabc", 1));
    assert(!Utils::isHexNumber("-0x0", 0));
    assert(!Utils::isHexNumber("-0x01", 1));
    assert(!Utils::isHexNumber("-0xab", 1));
    assert(!Utils::isHexNumber("-0xabc", 2));
    assert(!Utils::isHexNumber("-0xabc", 1));

    assert(!Utils::isHexNumber("", 3));
    assert(!Utils::isHexNumber(" ", 3));
}
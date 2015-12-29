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
    assert(Utils::isDecNumber("123456789", true));
    assert(Utils::isDecNumber("1", true));

    assert(!Utils::isDecNumber("1s234324", true));
    assert(!Utils::isDecNumber("1234324s", true));
    assert(!Utils::isDecNumber("s1234324", true));
    assert(!Utils::isDecNumber(" 1234324", true));
    assert(!Utils::isDecNumber("1234324 ", true));
    assert(!Utils::isDecNumber(" 1234324 ", true));

    assert(Utils::isDecNumber("-123456789", true));
    assert(Utils::isDecNumber("-1", true));

    assert(!Utils::isDecNumber("-1s234324", true));
    assert(!Utils::isDecNumber("-1234324s", true));
    assert(!Utils::isDecNumber("s-1234324", true));
    assert(!Utils::isDecNumber(" -1234324", true));
    assert(!Utils::isDecNumber("-1234324 ", true));
    assert(!Utils::isDecNumber(" -1234324 ", true));

    assert(Utils::isDecNumber("123456789", false));
    assert(Utils::isDecNumber("1", false));
    assert(!Utils::isDecNumber("-123456789", false));
    assert(!Utils::isDecNumber("-1", false));

    assert(!Utils::isDecNumber("1s234324", false));
    assert(!Utils::isDecNumber("1234324s", false));
    assert(!Utils::isDecNumber("s1234324", false));
    assert(!Utils::isDecNumber(" 1234324", false));
    assert(!Utils::isDecNumber("1234324 ", false));
    assert(!Utils::isDecNumber(" 1234324 ", false));

    assert(!Utils::isDecNumber("-1s234324", false));
    assert(!Utils::isDecNumber("-1234324s", false));
    assert(!Utils::isDecNumber("-s1234324", false));
    assert(!Utils::isDecNumber(" -1234324", false));
    assert(!Utils::isDecNumber("-1234324 ", false));
    assert(!Utils::isDecNumber(" -1234324 ", false));

}
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
#include <sexpr_parsing/SExprParser.h>

using namespace wasm_module;
using namespace wasm_module::sexpr;

int main() {
    std::string str = ";This is a comment\n"
            " (; this is a silly comment in brackets ;)"
            "(module (memory \"langes wort\" 0 1))\n"
            ";This is another comment\n"
            "\t(module)"
            "\n(module)"
            "\n(module)";

    CharacterStream stream(str);

    SExprParser parser(stream);

    SExpr expr = parser.parse();

    assert(expr.children().size() == 4);

    assert(expr.children()[0].children().size() == 2);
    assert(expr.children()[0][0] == "module");
    assert(expr.children()[0][1].children().size() == 4);
    assert(expr.children()[0][1][0] == "memory");
    assert(expr.children()[0][1][1] == "langes wort");
    assert(expr.children()[0][1][2] == "0");
    assert(expr.children()[0][1][3] == "1");

    assert(expr.children()[1][0] == "module");
    assert(expr.children()[2][0] == "module");
    assert(expr.children()[3][0] == "module");

}
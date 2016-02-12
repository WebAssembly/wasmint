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
#include <sexpr_parsing/ModuleParser.h>
#include <instructions/InstructionAddress.h>

using namespace wasm_module;
using namespace wasm_module::sexpr;

int main() {
    std::string str =
        "(module \n"
        "    (func $main (param)\n"
        "        (i32.add (i32.const 1) (i32.sub (i32.const 2) (i32.const 3)))\n"
        "    )\n"
        ")";

    StringCharacterStream stream(str);

    SExprParser parser(stream);

    SExpr expr = parser.parse();

    Module& module = *ModuleParser::parse(expr[0]);

    module.functions().front()->mainInstruction()->foreachChild([&](Instruction* instruction){
        InstructionAddress address = instruction->getAddress();

        const Instruction* instructionToTest = module.functions().front()->instruction(address);
        assert(instructionToTest == instruction);
    });


    delete &module;
}
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


#include <cassert>
#include <cstdint>
#include <binary_parsing/ByteStream.h>
#include <Module.h>
#include <binary_parsing/ModuleParser.h>
#include <memory>
#include <interpreter/MachineState.h>
#include <types/Int32.h>
#include <iostream>
#include <interpreter/Thread.h>

#define BLOCK 0x7
#define SET_LOCAL 0x5
#define PRINT 0x6
#define INT32_ADD 0x0
#define GET_LOCAL 0x4
#define CALL 0x1
#define LITERAL 0x2
#define INT32_LOAD 0x8
#define INT32_STORE 0x9

using namespace wasmint;

int main() {
    std::deque<uint8_t> data = {
            // Module header
            // unless specified otherwise, all numbers are LEB128 encoded

            // the names of modules that are required to run this modules
            0, // We don't depend on any for now

            // the opcode table
            10, // we use 10 instructions in this module
            // the string name of the instructions
            'i', 'n', 't', '3', '2', '.', 'a', 'd', 'd', '\0', // int32.add = 0x0
            'c', 'a', 'l', 'l', '_', 'd', 'i', 'r', 'e', 'c', 't', '\0', // call a function directly = 0x1
            'l', 'i', 't', 'e', 'r', 'a', 'l', '\0',           // literal = 0x2
            'i', 'n', 't', '3', '2', '.', 'd', 'i', 'v', '\0', // int32.div = 0x3
            'g', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // get_local = 0x4
            's', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // set_local = 0x5
            'p', 'r', 'i', 'n', 't', '\0', // debug opcode which prints to console = 0x6
            'b', 'l', 'o', 'c', 'k', '\0', // starts a block = 0x7
            'i', 'n', 't', '3', '2', '.', 'l', 'o', 'a', 'd', '\0', // int32.load = 0x8
            'i', 'n', 't', '3', '2', '.', 's', 't', 'o', 'r', 'e', '\0', // int32.store = 0x9

            // now the table for the used types
            2, // we use 2 types in this module
            'v', 'o', 'i', 'd', '\0',
            'i', 'n', 't', '3', '2', '\0',

            // now the table for the used functions
            0, // we use 0 external functions in this module

            // now the table for the used external globals
            0, // we use 0 external globals

            // now the table for the internal globals
            1, // we have 1 internal global
            0x1, 'g', 'v', 'a', 'r', '\0', // global int32 named "gvar"

            // now the section table
            1, // only one section
            1, // section 1 is program code (1 means program code, rest is undefined).
            121, // start offset of the section in this array

            // section 1
            2, // we have only two functions in this section

            // signature of the main function
            'm', 'a', 'i', 'n', '\0', // the name of the function
            1, // export function
            0, // return type
            0, // number of parameters
            // here the parameter types would be listed
            13,  // offset in this section

            // signature of the test function
            't', 'e', 's', 't', '\0', // the name of the function
            0, // don't export function
            0, // return type
            0, // number of parameters
            // here the parameter types would be listed
            23,  // offset in this section

            // the main function
            2, // number of locals
            0x1, // local variable 0x0 with type int32
            0x1, // local variable 0x1 with type int32

            INT32_ADD, LITERAL, 0x1, 2, LITERAL, 0x1, 3,

            // the test function
            1, // number of locals
            0x1, // local variable 0x0 with type int32

            BLOCK, 0x1, // we start a new block with 1 instructions in it
            PRINT, INT32_LOAD, LITERAL, 0x1, 2, // print the int32 with value 3 from the linear memory
    };

    wasm_module::binary::ByteStream stream(data);

    std::unique_ptr<wasm_module::Module> m;
    m.reset(wasm_module::binary::ModuleParser::parse(stream));

    assert(m->opcodeTable().getInstruction(0x0) == "int32.add");
    assert(m->opcodeTable().getInstruction(0x1) == "call_direct");
    assert(m->opcodeTable().getInstruction(0x2) == "literal");
    assert(m->opcodeTable().getInstruction(0x3) == "int32.div");
    assert(m->opcodeTable().getInstruction(0x4) == "get_local");
    assert(m->opcodeTable().getInstruction(0x5) == "set_local");
    assert(m->opcodeTable().getInstruction(0x6) == "print");
    assert(m->opcodeTable().getInstruction(0x7) == "block");
    assert(m->opcodeTable().getInstruction(0x8) == "int32.load");
    assert(m->opcodeTable().getInstruction(0x9) == "int32.store");

    assert(m->typeTable().getType(0x0) == wasm_module::Void::instance());
    assert(m->typeTable().getType(0x1) == wasm_module::Int32::instance());


    assert(m->sections().size() == 1);

    MachineState environment;
    environment.useModule(*m);
    Thread &thread = environment.createThread().startAtFunction("main");
    thread.stepUntilFinished();

    assert(wasm_module::Int32::getValue(thread.getInstructionState().result()) == 5);

}

#include <cassert>
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>
#include <memory>
#include <interpreter/RuntimeEnvironment.h>
#include <types/Int32.h>

#define BLOCK 0x7
#define SET_LOCAL 0x5
#define PRINT 0x6
#define INT32_ADD 0x0
#define GET_LOCAL 0x4
#define CALL 0x1
#define LITERAL 0x2
#define GET_GLOBAL 0x8
#define SET_GLOBAL 0x9

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
            'g', 'e', 't', '_', 'g', 'l', 'o', 'b', 'a', 'l', '\0', // get_global = 0x8
            's', 'e', 't', '_', 'g', 'l', 'o', 'b', 'a', 'l', '\0', // set_global = 0x9

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
            120, // start offset of the section in this array

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

            BLOCK, 0x6, // we start a new block with 6 instructions in it
                SET_GLOBAL, 0x0, LITERAL, 0x1, 32, // set our only global to 32
                SET_LOCAL, 0x0, LITERAL, 0x1, 3, // set_local the variable with index 0 to 3
                SET_LOCAL, 0x1, LITERAL, 0x1, 4, // set_local the variable with index 1 to 4
                PRINT, // print the result of
                    INT32_ADD, // int32.add with
                        GET_LOCAL, 0x0, // an variable at index 0x0 as first argument
                        GET_LOCAL, 0x1, // an variable at index 0x1 as second argument
                CALL, 0x1, // call the test function
                CALL, 0x1, // call the test function again

            // the test function
            1, // number of locals
            0x1, // local variable 0x0 with type int32

            BLOCK, 0x1, // we start a new block with 1 instruction in it
                PRINT, GET_GLOBAL, 0x0, // print the 32 from the global (that was set in main)
    };

    ByteStream stream(data);

    std::unique_ptr<Module> m;
    m.reset(ModuleParser::parse(stream));

    assert(m->opcodeTable().getInstruction(0x0) == "int32.add");
    assert(m->opcodeTable().getInstruction(0x1) == "call_direct");
    assert(m->opcodeTable().getInstruction(0x2) == "literal");
    assert(m->opcodeTable().getInstruction(0x3) == "int32.div");
    assert(m->opcodeTable().getInstruction(0x4) == "get_local");
    assert(m->opcodeTable().getInstruction(0x5) == "set_local");
    assert(m->opcodeTable().getInstruction(0x6) == "print");
    assert(m->opcodeTable().getInstruction(0x7) == "block");
    assert(m->opcodeTable().getInstruction(0x8) == "get_global");
    assert(m->opcodeTable().getInstruction(0x9) == "set_global");

    assert(m->typeTable().getType(0x0) == Void::instance());
    assert(m->typeTable().getType(0x1) == Int32::instance());


    assert(m->sections().size() == 1);

    RuntimeEnvironment environment;
    environment.useModule(*m);
    Thread& thread = environment.createThread().startAtFunction("main");
    thread.stepUntilFinished();

    // This module should print the number 7 in main and then 3232 in the two times we call the test function
    assert(environment.stdout() == "73232");

}

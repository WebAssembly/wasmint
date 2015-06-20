
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>

#define BLOCK 0x7
#define SET_LOCAL 0x5
#define PRINT 0x6
#define INT32_ADD 0x0
#define GET_LOCAL 0x4
#define CALL 0x1
#define LITERAL 0x2

int main() {
    std::deque<uint8_t> data = {
            // Module header
            // unless specified otherwise, all numbers are LEB128 encoded
            // first the opcode table
            8, // we use 8 instructions in this module
            // the string name of the instructions
            'i', 'n', 't', '3', '2', '.', 'a', 'd', 'd', '\0', // int32.add = 0x0
            'c', 'a', 'l', 'l', '\0',                          // call a function = 0x1
            'l', 'i', 't', 'e', 'r', 'a', 'l', '\0',           // literal = 0x2
            'i', 'n', 't', '3', '2', '.', 'd', 'i', 'v', '\0', // int32.div = 0x3
            'g', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // get_local = 0x4
            's', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // set_local = 0x5
            'p', 'r', 'i', 'n', 't', '\0', // debug opcode which prints to console = 0x6
            'b', 'l', 'o', 'c', 'k', '\0', // starts a block = 0x7

            // now the table for the used types
            2, // we use 2 types in this module
            'v', 'o', 'i', 'd', '\0',
            'i', 'n', 't', '3', '2', '\0',

            // now the table for the used functions
            0, // we use 0 external functions in this module

            // now the section table
            1, // only one section
            1, // section 1 is program code (1 means program code, 0 means data).
            82, // start offset of the section in this array

            // section 1
            2, // we have only two functions in this section

            // signature of the main function
            'm', 'a', 'i', 'n', '\0', // the name of the function
            0, // return type
            0, // number of parameters
            // here the parameter types would be listed
            13,  // offset in this section

            // signature of the test function
            't', 'e', 's', 't', '\0', // the name of the function
            0, // return type
            0, // number of parameters
            // here the parameter types would be listed
            13,  // offset in this section

            // the main function
            2, // number of locals
            0x1, // local variable 0x0 with type int32
            0x1, // local variable 0x1 with type int32

            BLOCK, 0x5, // we start a new block with 5 instructions in it
                SET_LOCAL, 0x0, LITERAL, 0x1, 3, // set_local the variable with index 0 to 3
                SET_LOCAL, 0x1, LITERAL, 0x1, 4, // set_local the variable with index 1 to 4
                PRINT, // print the result of
                    INT32_ADD, // int32.add with
                        GET_LOCAL, 0x0, // an variable at index 0x0 as first argument
                        GET_LOCAL, 0x1, // an variable at index 0x1 as second argument
                CALL, 0x1, // call the test function
                CALL, 0x1, // call the test function

            // the test function
            1, // number of locals
            0x1, // local variable 0x0 with type int32

            BLOCK, 0x2, // we start a new block with 2 instructions in it
                SET_LOCAL, 0x0, LITERAL, 0x1, 66, // set_local the variable with index 0 to 66
                PRINT, GET_LOCAL, 0x0, // print the 66
    };

    ByteStream stream(data);

    Module* m = ModuleParser::parse(stream);

    assert(m->opcodeTable().getInstruction(0x0) == "int32.add");
    assert(m->opcodeTable().getInstruction(0x1) == "call");
    assert(m->opcodeTable().getInstruction(0x2) == "literal");
    assert(m->opcodeTable().getInstruction(0x3) == "int32.div");
    assert(m->opcodeTable().getInstruction(0x4) == "get_local");
    assert(m->opcodeTable().getInstruction(0x5) == "set_local");
    assert(m->opcodeTable().getInstruction(0x6) == "print");

    assert(m->typeTable().getType(0x0) == Void::instance());
    assert(m->typeTable().getType(0x1) == Int32::instance());


    assert(m->sections().size() == 1);

    Environment environment;
    environment.useModule(*m);
    environment.callFunction("main");

    // This module should print the number 7 in mainand then 6666 in the two times we call the test function
    assert(environment.stdout() == "76666");

}

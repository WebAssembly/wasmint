
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>

int main() {
    std::deque<uint8_t> data = {
            // Module header
            // unless specified otherwise, all numbers are LEB128 encoded
            // first the opcode table
            7, // we use 7 instructions
            // the string name of the instructions
            'i', 'n', 't', '3', '2', '.', 'a', 'd', 'd', '\0', // int32.add = 0x0
            'i', 'n', 't', '3', '2', '.', 's', 'u', 'b', '\0', // int32.sub = 0x1
            'i', 'n', 't', '3', '2', '.', 'm', 'u', 'l', '\0', // int32.mul = 0x2
            'i', 'n', 't', '3', '2', '.', 'd', 'i', 'v', '\0', // int32.div = 0x3
            'g', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // get_local = 0x4
            's', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // set_local = 0x5
            'p', 'r', 'i', 'n', 't', '\0', // debug opcode which prints to console = 0x6

            2, // we use 2 types
            'v', 'o', 'i', 'd', '\0',
            'i', 'n', 't', '3', '2', '\0',

            // now the section table
            1, // only one section
            1, // section 1 is program code (= 1).
            82, // start offset in this array is 70

            // section 1
            1, // we have only one function
            'm', 'a', 'i', 'n', '\0', // the name of the function
            1, // return type
            0, // number of parameters
            // here the parameter types would be listed
            13,  // offset in this section

            // the main function
            2, // number of locals
            0x1, // local variable 0x0 with type int32
            0x1, // local variable 0x1 with type int32

            0x6, // print the result of
                0x0, // int32.add with
                    0x4, 0x0, // an variable at index 0x0 as first argument
                    0x4, 0x1, // an variable at index 0x1 as second argument
    };

    ByteStream stream(data);

    Module* m = ModuleParser::parse(stream);

    assert(m->sections().size() == 1);
    assert(m->opcodeTable().getInstruction(0x0) == "int32.add");
    assert(m->opcodeTable().getInstruction(0x1) == "int32.sub");
    assert(m->opcodeTable().getInstruction(0x2) == "int32.mul");
    assert(m->opcodeTable().getInstruction(0x3) == "int32.div");
    assert(m->opcodeTable().getInstruction(0x4) == "get_local");
    assert(m->opcodeTable().getInstruction(0x5) == "set_local");
    assert(m->opcodeTable().getInstruction(0x6) == "print");

    assert(m->typeTable().getType(0x0) == "void");
    assert(m->typeTable().getType(0x1) == "int32");

    Environment environment;

}

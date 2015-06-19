
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>

int main() {
    std::deque<uint8_t> data = {
            // Module header

            // first the opcode table
            7, // we use 7 instructions (LEB128 integer)
            // the string name of the instructions
            'i', 'n', 't', '3', '2', '.', 'a', 'd', 'd', '\0', // int32.add = 0x0
            'i', 'n', 't', '3', '2', '.', 's', 'u', 'b', '\0', // int32.sub = 0x1
            'i', 'n', 't', '3', '2', '.', 'm', 'u', 'l', '\0', // int32.mul = 0x2
            'i', 'n', 't', '3', '2', '.', 'd', 'i', 'v', '\0', // int32.div = 0x3
            'g', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // get_local = 0x4
            's', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // set_local = 0x5
            'p', 'r', 'i', 'n', 't', '\0', // debug opcode which prints to console = 0x6

            // now the section table
            1, // only one section (LEB128 integer)
            1, // section 1 is code (= 1). Single uint8 entry
            70, // start offset in this array is 70 (LEB128 integer)

            0 // start of section 1
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



}

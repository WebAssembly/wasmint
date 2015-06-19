
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>

int main() {
    std::deque<uint8_t> data = {
            // Module header

            // first the opcode table
            7, // we use 7 opcodes (LEB128-Encoded integer)
            'i', 'n', 't', '3', '2', '.', 'a', 'd', 'd', '\0', // int32.add
            'i', 'n', 't', '3', '2', '.', 's', 'u', 'b', '\0', // int32.add
            'i', 'n', 't', '3', '2', '.', 'm', 'u', 'l', '\0', // int32.add
            'i', 'n', 't', '3', '2', '.', 'd', 'i', 'v', '\0', // int32.add
            'g', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // get_local
            's', 'e', 't', '_', 'l', 'o', 'c', 'a', 'l', '\0', // set_local
            'p', 'r', 'i', 'n', 't', '\0', // debug opcode which prints to console

            // now the section table
            0, 0, 0, 1, // only one section
            1, // section 1 is code (= 1). Data would be 0
            0, 0, 0, 76, // start offset in this array is 76


            0 // start of section 1
    };

    ByteStream stream(data);

    Module* m = ModuleParser::parse(stream);

    assert(m->sections().size() == 1);
}

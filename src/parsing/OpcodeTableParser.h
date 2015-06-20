

#ifndef WASMINT_OPCODETABLEPARSER_H
#define WASMINT_OPCODETABLEPARSER_H

#include <string>
#include <vector>

#include <OpcodeTable.h>
#include "ByteStream.h"

class OpcodeTableParser {

    ByteStream& stream;
    OpcodeTable opcodeTable;

protected:
    OpcodeTableParser(ByteStream& stream) : stream(stream) {
    }

    void parse() {
        uint32_t numberOfOpcodes = stream.popULEB128();
        for(uint32_t i = 0; i < numberOfOpcodes; i++) {
            opcodeTable.addInstruction(i, stream.readCString());
        }
    }

    OpcodeTable getParsedTable() {
        return opcodeTable;
    }

public:
    static OpcodeTable parse(ByteStream& stream) {
        OpcodeTableParser parser(stream);
        parser.parse();
        return parser.getParsedTable();
    }
};


#endif //WASMINT_OPCODETABLEPARSER_H

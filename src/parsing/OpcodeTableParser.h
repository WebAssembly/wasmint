//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_OPCODETABLEPARSER_H
#define WASMINT_OPCODETABLEPARSER_H

#include <string>
#include <vector>

#include <OpcodeTable.h>
#include "ByteStream.h"

class OpcodeTableParser {

    ByteStream& stream;
    std::vector<std::string> opcodeNames;

protected:
    OpcodeTableParser(ByteStream& stream) : stream(stream) {
    }

    std::string readCString() {
        std::string result = "";
        while (stream.peekChar() != 0) {
            result.push_back((unsigned char) stream.popChar());
        }
        // remove the \0 at the end
        stream.popChar();
        return result;
    }

    void parse() {
        uint32_t numberOfOpcodes = stream.popLEB128();
        opcodeNames.resize(numberOfOpcodes);
        for(uint32_t i = 0; i < numberOfOpcodes; i++) {
            opcodeNames[i] = readCString();
        }
    }

    OpcodeTable getParsedTable() {

    }

public:
    static OpcodeTable parse(ByteStream& stream) {
        OpcodeTableParser parser(stream);
        parser.parse();
        return parser.getParsedTable();
    }
};


#endif //WASMINT_OPCODETABLEPARSER_H

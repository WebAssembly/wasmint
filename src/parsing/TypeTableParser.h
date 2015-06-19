//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_TYPETABLEPARSER_H
#define WASMINT_TYPETABLEPARSER_H

#include "ByteStream.h"
#include <cstdint>
#include <string>
#include <TypeTable.h>

class TypeTableParser {
    ByteStream& stream;
    TypeTable typeTable;

protected:
    TypeTableParser(ByteStream& stream) : stream(stream) {
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
        uint32_t numberOfTypes = stream.popLEB128();
        for(uint32_t i = 0; i < numberOfTypes; i++) {
            typeTable.addType(i, readCString());
        }
    }

    TypeTable getParsedTable() {
        return typeTable;
    }

public:
    static TypeTable parse(ByteStream& stream) {
        TypeTableParser parser(stream);
        parser.parse();
        return parser.getParsedTable();
    }
};


#endif //WASMINT_TYPETABLEPARSER_H

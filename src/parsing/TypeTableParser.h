

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

    void parse() {
        uint32_t numberOfTypes = stream.popULEB128();
        for(uint32_t i = 0; i < numberOfTypes; i++) {
            typeTable.addType(i, stream.readCString());
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

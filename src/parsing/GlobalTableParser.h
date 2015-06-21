//
// Created by teemperor on 21.06.15.
//

#ifndef WASMINT_GLOBALTABLEPARSER_H
#define WASMINT_GLOBALTABLEPARSER_H


#include <GlobalTable.h>
#include <TypeTable.h>

class GlobalTableParser {

    ByteStream& stream;
    GlobalTable globalTable;
    TypeTable& typeTable_;

protected:
    GlobalTableParser(ByteStream& stream, TypeTable& typeTable)
            : stream(stream), typeTable_(typeTable) {
    }

    void parseExternGlobals() {
        uint32_t numberOfExternalGlobals = stream.popULEB128();
        for(uint32_t i = 0; i < numberOfExternalGlobals; i++) {
            uint32_t typeData = stream.popULEB128();
            std::string name = stream.readCString();

            Type* type = typeTable_.getType(typeData);

            globalTable.addGlobal(Global(name, type), false);
        }
    }

    void parseInternGlobals() {
        uint32_t numberOfInternalGlobals = stream.popULEB128();
        for(uint32_t i = 0; i < numberOfInternalGlobals; i++) {
            uint32_t typeData = stream.popULEB128();
            std::string name = stream.readCString();

            Type* type = typeTable_.getType(typeData);

            globalTable.addGlobal(Global(name, type), true);

        }
    }


    void parse() {
        parseExternGlobals();
        parseInternGlobals();
    }

    GlobalTable getParsedTable() {
        return globalTable;
    }

public:
    static GlobalTable parse(ByteStream& stream, TypeTable& typeTable) {
        GlobalTableParser parser(stream, typeTable);
        parser.parse();
        return parser.getParsedTable();
    }
};


#endif //WASMINT_GLOBALTABLEPARSER_H

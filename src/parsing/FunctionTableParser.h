//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_FUNCTIONTABLEPARSER_H
#define WASMINT_FUNCTIONTABLEPARSER_H


#include <FunctionTable.h>

class FunctionTableParser {

    ByteStream& stream;
    FunctionTable functionTable;

protected:
    FunctionTableParser(ByteStream& stream) : stream(stream) {
    }

    void parse() {
        uint32_t numberOfOpcodes = stream.popULEB128();
        for(uint32_t i = 0; i < numberOfOpcodes; i++) {
            functionTable.addFunctionSignature(FunctionSignature());
        }
    }

    FunctionTable getParsedTable() {
        return functionTable;
    }

public:
    static FunctionTable parse(ByteStream& stream) {
        FunctionTableParser parser(stream);
        parser.parse();
        return parser.getParsedTable();
    }
};


#endif //WASMINT_FUNCTIONTABLEPARSER_H

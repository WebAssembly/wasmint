//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_TYPETABLE_H
#define WASMINT_TYPETABLE_H

#include <string>
#include <cstdint>
#include <map>
#include <types/Void.h>
#include <types/Int32.h>
#include "Type.h"

class UnknownLocalTypeCode : public std::exception {};

class TypeTable {


    std::map<uint32_t, std::string> instructionsByLocalOpcode;

public:
    TypeTable() {
    }

    void addType(uint32_t localTypeCode, std::string name) {
        instructionsByLocalOpcode[localTypeCode] = name;
    }

    Type* getType(uint32_t localTypeCode) {
        std::string typeName = getTypeName(localTypeCode);
        if (typeName == "void") {
            return Void::instance();
        } else if (typeName == "int32") {
            return Int32::instance();
        }
    }

    std::string getTypeName(uint32_t localTypeCode) {
        auto result = instructionsByLocalOpcode.find(localTypeCode);
        if (result == instructionsByLocalOpcode.end()) {
            throw UnknownLocalTypeCode();
        } else {
            return result->second;
        }
    }
};


#endif //WASMINT_TYPETABLE_H

//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_TYPETABLE_H
#define WASMINT_TYPETABLE_H

#include <string>
#include <cstdint>
#include <map>

class UnknownLocalTypeCode : public std::exception {};

class TypeTable {


    std::map<uint32_t, std::string> instructionsByLocalOpcode;

public:
    TypeTable() {
    }

    void addType(uint32_t localTypeCode, std::string name) {
        instructionsByLocalOpcode[localTypeCode] = name;
    }

    std::string getInstruction(uint32_t localTypeCode) {
        auto result = instructionsByLocalOpcode.find(localTypeCode);
        if (result == instructionsByLocalOpcode.end()) {
            throw UnknownLocalTypeCode();
        } else {
            return result->second;
        }
    }
};


#endif //WASMINT_TYPETABLE_H

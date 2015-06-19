//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_OPCODETABLE_H
#define WASMINT_OPCODETABLE_H

#include <cstdint>
#include <string>
#include <map>

class UnknownLocalOpcode : public std::exception {};

class OpcodeTable {

    std::map<uint32_t, std::string> instructionsByLocalOpcode;

public:
    OpcodeTable() {
    }

    void addInstruction(uint32_t localOpcode, std::string name) {
        instructionsByLocalOpcode[localOpcode] = name;
    }

    std::string getInstruction(uint32_t localOpcode) {
        auto result = instructionsByLocalOpcode.find(localOpcode);
        if (result == instructionsByLocalOpcode.end()) {
            throw UnknownLocalOpcode();
        } else {
            return result->second;
        }
    }
};


#endif //WASMINT_OPCODETABLE_H

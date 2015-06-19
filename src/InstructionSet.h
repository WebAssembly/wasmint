//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_INSTRUCTIONSET_H
#define WASMINT_INSTRUCTIONSET_H

#include <string>
#include <instructions/I32Add.h>
#include "Instruction.h"

class UnknownInstructionName : public std::exception {};

class InstructionSet {

public:
    static Instruction* getInstruction(std::string name) {
        if (name == "int32.add") {
            return new I32Add();
        } else {
            throw UnknownInstructionName();
        }
    }
};


#endif //WASMINT_INSTRUCTIONSET_H

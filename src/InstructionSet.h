//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_INSTRUCTIONSET_H
#define WASMINT_INSTRUCTIONSET_H

#include <string>
#include <instructions/I32Add.h>
#include <parsing/ByteStream.h>
#include <instructions/Print.h>
#include <instructions/GetLocal.h>
#include "Instruction.h"

class UnknownInstructionName : public std::exception {};

class InstructionSet {

public:
    static Instruction* getInstruction(std::string name, ByteStream& stream) {
        if (name == "int32.add") {
            return new I32Add();
        } else if (name == "print") {
            return new Print();
        } else if (name == "get_local") {
            return new GetLocal(stream);
        } else {
            throw UnknownInstructionName();
        }
    }
};


#endif //WASMINT_INSTRUCTIONSET_H

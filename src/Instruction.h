//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_OPCODE_H
#define WASMINT_OPCODE_H


#include <cstdint>
#include <vector>

#include "Type.h"
#include "Variable.h"

class Instruction {

public:
    virtual std::vector<Type> childrenTypes() = 0;
    virtual Type returnType() = 0;

    virtual Variable execute() = 0;

};


#endif //WASMINT_OPCODE_H

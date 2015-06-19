//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_OPCODE_H
#define WASMINT_OPCODE_H


#include <cstdint>
#include <vector>
#include <interpreter/Environment.h>

#include "Type.h"
#include "Variable.h"

class Instruction {

    std::vector<Instruction*> children_;

public:
    virtual void children(std::vector<Instruction*> newChildren) {
        children_ = newChildren;
    }
    std::vector<Instruction*> children() {
        return children_;
    }

    virtual std::vector<Type*> childrenTypes() = 0;
    virtual Type* returnType() = 0;

    virtual Variable execute(Environment& env) = 0;

};


#endif //WASMINT_OPCODE_H

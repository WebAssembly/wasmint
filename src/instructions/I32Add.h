//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_I32ADD_H
#define WASMINT_I32ADD_H


#include "Instruction.h"

class I32Add : public Instruction {

public:
    virtual std::vector<Type> childrenTypes() {
        return {Type::Int32(), Type::Int32()};
    }

    virtual Type returnType() {
        return Type::Int32();
    }
};


#endif //WASMINT_I32ADD_H

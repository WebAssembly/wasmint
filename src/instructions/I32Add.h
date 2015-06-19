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

    virtual Variable execute(Environment& env) {
        Variable left = children().at(0)->execute(env);
        Variable right = children().at(1)->execute(env);
        return Variable(Type::Int32(), left.int32Value() + right.int32Value());
    }
};


#endif //WASMINT_I32ADD_H

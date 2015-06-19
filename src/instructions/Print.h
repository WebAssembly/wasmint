//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_PRINT_H
#define WASMINT_PRINT_H

#include <Instruction.h>

class Print : public Instruction {
public:

    virtual std::vector<Type> childrenTypes() {
        return {};
    }

    virtual Type returnType() {
        return Type::Int32();
    }

    virtual Variable execute(Environment& env) {
        return Variable(Type::Void(), 0);
    }
};


#endif //WASMINT_PRINT_H

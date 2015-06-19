//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_PRINT_H
#define WASMINT_PRINT_H

#include <Instruction.h>
#include <iostream>

class Print : public Instruction {
public:
    virtual std::vector<Type> childrenTypes() {
        return {Type::Int32()};
    }

    virtual Type returnType() {
        return Type::Void();
    }

    virtual Variable execute(Environment& env) {
        Variable v = children().at(0)->execute(env);
        std::cout << v.int32Value() << std::endl;

        return Variable(Type::Void(), 0);
    }
};


#endif //WASMINT_PRINT_H

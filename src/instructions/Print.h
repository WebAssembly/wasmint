//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_PRINT_H
#define WASMINT_PRINT_H

#include <Instruction.h>
#include <iostream>
#include <types/Int32.h>

class Print : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance()};
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Environment& env) {
        Variable v = children().at(0)->execute(env);
        env.print(std::to_string(Int32::getValue(v)));

        return Variable();
    }
};


#endif //WASMINT_PRINT_H

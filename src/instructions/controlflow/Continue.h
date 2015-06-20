//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_CONTINUE_H
#define WASMINT_CONTINUE_H


#include <Instruction.h>

class CalledContinue {};

class Continue : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {};
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Environment& env) {
        throw CalledContinue();
    }
};



#endif //WASMINT_CONTINUE_H

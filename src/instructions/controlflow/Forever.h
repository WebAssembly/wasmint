//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_FOREVER_H
#define WASMINT_FOREVER_H


#include <Instruction.h>
#include "Break.h"
#include "Continue.h"

class Forever : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Void::instance()};
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Environment& env) {
        try {
            while (true) {
                try {
                    children().at(0)->execute(env);
                } catch (CalledContinue) {

                }
            }
        } catch (CalledBreak) {

        }
        return Variable();
    }
};



#endif //WASMINT_FOREVER_H

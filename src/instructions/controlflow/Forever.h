

#ifndef WASMINT_FOREVER_H
#define WASMINT_FOREVER_H


#include <instructions/Instruction.h>
#include "Break.h"
#include "Continue.h"

class Forever : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Void::instance()};
    }

    virtual std::string name() {
        return "forever";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Thread &thread) {
        try {
            while (true) {
                try {
                    children().at(0)->execute(thread);
                } catch (CalledContinue) {

                }
            }
        } catch (CalledBreak) {

        }
        return Variable();
    }
};



#endif //WASMINT_FOREVER_H

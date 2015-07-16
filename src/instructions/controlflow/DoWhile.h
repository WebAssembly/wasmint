

#ifndef WASMINT_DOWHILE_H
#define WASMINT_DOWHILE_H


#include <instructions/Instruction.h>
#include <types/Int32.h>
#include "Break.h"
#include "Continue.h"

class DoWhile : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance(), Void::instance()};
    }

    virtual std::string name() {
        return "do_while";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Thread &thread) {
        Variable condition;
        try {
            while (true) {
                try {
                    children().at(1)->execute(thread);
                } catch (CalledContinue) {

                }
                condition = children().at(0)->execute(thread);
                if (!Int32::getValue(condition)) {
                    break;
                }
            }
        } catch (CalledBreak) {

        }
        return Variable();
    }
};



#endif //WASMINT_DOWHILE_H

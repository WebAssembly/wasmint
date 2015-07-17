

#ifndef WASMINT_IF_H
#define WASMINT_IF_H


#include <instructions/Instruction.h>
#include <types/Int32.h>

class If : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance(), Void::instance()};
    }

    virtual std::string name() {
        return "if";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual StepResult execute(Thread &thread) {
/*
        Variable condition = children().at(0)->execute(thread);

        if (Int32::getValue(condition)) {
            children().at(1)->execute(thread);
        }

        return Variable(); */
        return StepResult();
    }
};



#endif //WASMINT_IF_H

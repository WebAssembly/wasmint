

#ifndef WASMINT_CONTINUE_H
#define WASMINT_CONTINUE_H


#include <instructions/Instruction.h>

class CalledContinue {};

class Continue : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {};
    }
    virtual std::string name() {
        return "continue";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual StepResult execute(Thread &thread) {
        return StepResult(Signal::Continue);
    }
};



#endif //WASMINT_CONTINUE_H

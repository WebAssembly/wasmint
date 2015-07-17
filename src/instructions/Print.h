

#ifndef WASMINT_PRINT_H
#define WASMINT_PRINT_H

#include <instructions/Instruction.h>
#include <iostream>
#include <types/Int32.h>

class Print : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance()};
    }

    virtual std::string name() {
        return "print";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual StepResult execute(Thread &thread) {
        InstructionState& state = thread.getInstructionState();
        switch(state.state()) {
            case 0:
                return children().at(0);
            default:
                thread.runtimeEnvironment().print(std::to_string(Int32::getValue(state.results().at(0))));
                return StepResult();

        }
    }
};


#endif //WASMINT_PRINT_H

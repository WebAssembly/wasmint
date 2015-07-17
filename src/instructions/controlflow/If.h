

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
        InstructionState& state = thread.getInstructionState();
        switch(state.state()) {
            case 0:
                return children().at(0);
            case 1:
                if (Int32::getValue(state.results().front()) != 0) {
                    return children().at(1);
                }
            default:
                return StepResult();

        }
    }
};



#endif //WASMINT_IF_H



#ifndef WASMINT_I32SUB_H
#define WASMINT_I32SUB_H


#include <instructions/Instruction.h>
#include <types/Int32.h>

class I32Sub : public Instruction {

public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance(), Int32::instance()};
    }

    virtual std::string name() {
        return "int32.sub";
    }


    virtual Type* returnType() {
        return Int32::instance();
    }

    virtual StepResult execute(Thread &thread) {

        InstructionState& state = thread.getInstructionState();
        switch(state.state()) {
            case 0:
                return StepResult(children().at(0));
            case 1:
                return StepResult(children().at(1));
            default:
                int32_t left = Int32::getValue(state.results().at(0));
                int32_t right = Int32::getValue(state.results().at(1));

                Variable result = Variable(Int32::instance());
                Int32::setValue(result, left - right);
                return result;
        }
    }
};


#endif //WASMINT_I32SUB_H



#ifndef WASMINT_STOREHEAP_H
#define WASMINT_STOREHEAP_H


#include <instructions/Instruction.h>
#include <types/Int32.h>

class Int32Store : public Instruction {

public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance(), Int32::instance()};
    }

    virtual Type* returnType() {
        return Int32::instance();
    }

    virtual std::string name() {
        return "int32.store";
    }

    virtual StepResult execute(Thread &thread) {

        InstructionState& state = thread.getInstructionState();
        switch(state.state()) {
            case 0:
                return StepResult(children().at(0));
            case 1:
                return StepResult(children().at(1));
            default:

                uint32_t offset = static_cast<uint32_t>(Int32::getValue(state.results().at(0)));

                Variable value = state.results().at(1);
                thread.runtimeEnvironment().heap().setBytes(offset, value.data());
                return StepResult(value);
        }
    }
};


#endif //WASMINt_STOREHEAP_H

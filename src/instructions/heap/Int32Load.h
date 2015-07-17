

#ifndef WASMINT_LOADHEAP_H
#define WASMINT_LOADHEAP_H


#include <instructions/Instruction.h>
#include <types/Int32.h>

class Int32Load : public Instruction {

public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance()};
    }

    virtual Type* returnType() {
        return Int32::instance();
    }

    virtual std::string name() {
        return "int32.load";
    }

    virtual StepResult execute(Thread &thread) {

        InstructionState& state = thread.getInstructionState();
        switch(state.state()) {
            case 0:
                return StepResult(children().at(0));
            default:
                uint32_t offset = static_cast<uint32_t>(Int32::getValue(state.results().back()));

                std::vector<uint8_t> bytes = thread.runtimeEnvironment().heap().getBytes(offset, Int32::instance()->size());

                Variable result = Variable(Int32::instance()->localType());
                result.setValue(bytes);
                return result;
        }

    }
};


#endif //WASMINT_LOADHEAP_H

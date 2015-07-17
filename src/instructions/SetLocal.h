

#ifndef WASMINT_SETLOCAL_H
#define WASMINT_SETLOCAL_H

#include <cstdint>
#include <instructions/Instruction.h>
#include <parsing/ByteStream.h>
#include <types/Int32.h>
#include <FunctionContext.h>

class SetLocal : public Instruction {

    uint32_t localIndex;
    Type* expectedType;

public:
    SetLocal(ByteStream& stream, FunctionContext& functionContext) {
        localIndex = stream.popULEB128();
        expectedType = functionContext.pureLocals().at(localIndex);
    }

    virtual std::string name() {
        return "set_local";
    }

    virtual std::vector<Type*> childrenTypes() {
        return {expectedType};
    }

    virtual Type* returnType() {
        return expectedType;
    }

    virtual StepResult execute(Thread &thread) {
        InstructionState& state = thread.getInstructionState();
        switch(state.state()) {
            case 0:
                return children().at(0);
            default:
                Variable result = thread.variable(localIndex) = state.results().at(0);
                return result;

        }
    }
};


#endif //WASMINT_SETLOCAL_H

//
// Created by teemperor on 21.06.15.
//

#ifndef WASMINT_SETGLOBAL_H
#define WASMINT_SETGLOBAL_H


#include "Instruction.h"

class SetGlobal : public Instruction {

    Type* expectedType;
    std::string globalName;

public:
    SetGlobal(ByteStream& stream, ModuleContext& context) {
        uint32_t globalIndex = stream.popULEB128();
        expectedType = context.globalTable().getGlobal(globalIndex).type();
        globalName = context.globalTable().getGlobal(globalIndex).name();
    }

    virtual std::string name() {
        return "set_global";
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
                Variable result = thread.runtimeEnvironment().global(globalName) = state.results().at(0);
                return result;

        }
    }
};



#endif //WASMINT_SETGLOBAL_H

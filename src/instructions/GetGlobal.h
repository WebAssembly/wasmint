//
// Created by teemperor on 21.06.15.
//

#ifndef WASMINT_GETGLOBAL_H
#define WASMINT_GETGLOBAL_H


#include "Instruction.h"

class GetGlobal : public Instruction {

    Type* returnType_;
    std::string globalName;

public:
    GetGlobal(ByteStream& stream, ModuleContext& context) {
        uint32_t globalIndex = stream.popULEB128();
        returnType_ = context.globalTable().getGlobal(globalIndex).type();
        globalName = context.globalTable().getGlobal(globalIndex).name();
    }

    virtual std::string name() {
        return "get_global";
    }

    virtual std::vector<Type*> childrenTypes() {
        return {};
    }

    virtual Type* returnType() {
        return returnType_;
    }

    virtual StepResult execute(Thread &thread) {
        return thread.runtimeEnvironment().global(globalName);
    }
};



#endif //WASMINT_GETGLOBAL_H

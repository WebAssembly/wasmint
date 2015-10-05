//
// Created by teemperor on 21.06.15.
//

#ifndef WASMINT_SETGLOBAL_H
#define WASMINT_SETGLOBAL_H


#include <ModuleContext.h>
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

    virtual Variable execute(Thread& thread);
};



#endif //WASMINT_SETGLOBAL_H

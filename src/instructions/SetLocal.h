//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_SETLOCAL_H
#define WASMINT_SETLOCAL_H

#include <cstdint>
#include <instructions/Instruction.h>
#include <parsing/ByteStream.h>
#include <types/Int32.h>

class SetLocal : public Instruction {

    uint32_t localIndex;
    Type* expectedType;

public:
    SetLocal(ByteStream& stream, FunctionContext& functionContext) {
        localIndex = stream.popULEB128();
        expectedType = functionContext.locals().at(localIndex);
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

    virtual Variable execute(Environment& env) {
        return env.variable(localIndex) = children().at(0)->execute(env);
    }
};


#endif //WASMINT_SETLOCAL_H

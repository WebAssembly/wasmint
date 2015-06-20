//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_FUNCTIONCALL_H
#define WASMINT_FUNCTIONCALL_H


#include <Instruction.h>
#include <ModuleContext.h>

class FunctionCall : public Instruction {

    FunctionSignature functionSignature;

public:
    FunctionCall(ByteStream& stream, ModuleContext& context) {
        functionSignature = context.functionTable().getFunctionSignature(stream.popLEB128());
    }

    virtual std::vector<Type*> childrenTypes() {
        return functionSignature.parameters();
    }

    virtual Type* returnType() {
        return functionSignature.returnType();
    }

    virtual Variable execute(Environment& env) {
        return env.callFunction(functionSignature.name());
    }
};


#endif //WASMINT_FUNCTIONCALL_H

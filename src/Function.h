

#ifndef WASMINT_FUNCTION_H
#define WASMINT_FUNCTION_H

#include <string>
#include <vector>

#include "types/Type.h"
#include "Variable.h"
#include "FunctionSignature.h"
#include "FunctionContext.h"

class Instruction;
class Environment;

class Function : public FunctionSignature, public FunctionContext {

    Instruction* mainInstruction;

public:
    Function(std::string name, Type* returnType, std::vector<Type*> parameterTypes, FunctionContext& context, Instruction* mainInstruction)
        : FunctionContext(context), mainInstruction(mainInstruction), FunctionSignature(name, returnType, parameterTypes)
    {

    }


    Variable execute(Environment& environment);
};


#endif //WASMINT_FUNCTION_H

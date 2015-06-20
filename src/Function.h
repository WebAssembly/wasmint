//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_FUNCTION_H
#define WASMINT_FUNCTION_H

#include <string>
#include <vector>

#include "Type.h"
#include "Variable.h"
#include "FunctionSignature.h"

class Instruction;
class Environment;

class Function : public FunctionSignature {

    Instruction* mainInstruction;
    std::vector<Type*> locals_;

public:
    Function(std::string name, Type* returnType, std::vector<Type*> parameterTypes, std::vector<Type*> locals, Instruction* mainInstruction)
        : locals_(locals), mainInstruction(mainInstruction), FunctionSignature(name, returnType, parameterTypes)
    {

    }

    std::vector<Type*> locals() {
        return locals_;
    }

    Variable execute(Environment& environment);
};


#endif //WASMINT_FUNCTION_H

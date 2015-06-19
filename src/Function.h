//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_FUNCTION_H
#define WASMINT_FUNCTION_H

#include <string>
#include <vector>

#include "Type.h"

class Instruction;
class Environment;

class Function {

    std::string name_;
    Type returnType_;
    std::vector<Type> parameterTypes_;
    Instruction* mainInstruction;
    std::vector<Type> locals_;

public:
    Function(std::string name, Type returnType, std::vector<Type> parameterTypes, std::vector<Type> locals, Instruction* mainInstruction)
        : name_(name), returnType_(returnType), parameterTypes_(parameterTypes),
          mainInstruction(mainInstruction), locals_(locals)
    {
    }

    std::vector<Type> locals() {
        return locals_;
    }

    std::string name() {
        return name_;
    }

    void execute(Environment& environment);
};


#endif //WASMINT_FUNCTION_H

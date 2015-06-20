//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_FUNCTIONSIGNATURE_H
#define WASMINT_FUNCTIONSIGNATURE_H

#include <string>
#include <vector>

#include "types/Type.h"
#include "Variable.h"

class FunctionSignature {

    std::string name_;
    Type* returnType_ = Void::instance();
    std::vector<Type*> parameterTypes_;

public:
    FunctionSignature() {

    }

    FunctionSignature(std::string name, Type* returnType, std::vector<Type*> parameterTypes)
    : name_(name), returnType_(returnType), parameterTypes_(parameterTypes) {
    }

    Type* returnType() {
        return returnType_;
    }

    std::vector<Type*> parameters() {
        return parameterTypes_;
    }


    std::string name() {
        return name_;
    }
};


#endif //WASMINT_FUNCTIONSIGNATURE_H

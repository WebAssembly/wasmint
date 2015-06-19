//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_FUNCTION_H
#define WASMINT_FUNCTION_H

#include <string>
#include <vector>

#include "Type.h"

class Function {

    std::string name_;
    Type returnType_;
    std::vector<Type> parameterTypes_;

public:
    Function(std::string name, Type returnType, std::vector<Type> parameterTypes)
        : name_(name), returnType_(returnType), parameterTypes_(parameterTypes) {

    }
};


#endif //WASMINT_FUNCTION_H

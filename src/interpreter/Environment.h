//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_ENVIRONMENT_H
#define WASMINT_ENVIRONMENT_H


#include <Variable.h>
#include <vector>

class Environment {

    std::vector<Variable> variables_;

public:
    Environment(std::vector<Type> variableTypes) {
        for(Type type : variableTypes) {
            variables_.push_back(Variable(type, 0));
        }
    }

    Variable& variable(uint32_t index) {
        return variables_.at(index);
    }

};


#endif //WASMINT_ENVIRONMENT_H

//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_ENVIRONMENT_H
#define WASMINT_ENVIRONMENT_H


#include <Variable.h>
#include <vector>
#include <stack>
#include <Function.h>

class Environment {

    std::stack<std::vector<Variable>> stack;

    void createLocals(std::vector<Type> variableTypes) {
        stack.push(std::vector<Variable>());
        for(Type& type : variableTypes) {
            stack.top().push_back(Variable(type, 0));
        }
    }

public:
    Environment() {

    }

    void enterFunction(Function& function) {
        createLocals(function.locals());
    }

    Variable& variable(uint32_t index) {
        return stack.top().at(index);
    }

    void leaveFunction() {
        stack.pop();
    }

};


#endif //WASMINT_ENVIRONMENT_H

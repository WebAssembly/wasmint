//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_ENVIRONMENT_H
#define WASMINT_ENVIRONMENT_H


#include <Variable.h>
#include <vector>
#include <stack>
#include <Function.h>
#include <map>
#include <Module.h>

class NoFunctionWithName : public ExceptionWithMessage {
public:
    NoFunctionWithName(std::string message) : ExceptionWithMessage(message) {}
};

class Environment {

    std::stack<std::vector<Variable>> stack;

    std::map<std::string, Function*> functions_;

    void createLocals(std::vector<Type*> variableTypes) {
        stack.push(std::vector<Variable>());
        for(Type* type : variableTypes) {
            stack.top().push_back(Variable(type));
        }
    }

    std::string stdout_;


    void enterFunction(Function& function) {
        createLocals(function.locals());
    }

    void leaveFunction() {
        stack.pop();
    }

public:
    Environment() {

    }

    void useModule(Module& module) {
        std::vector<Function*> functions = module.functions();
        for(Function* function : functions) {
            functions_[function->name()] = function;
        }
    }

    Variable callFunction(std::string functionName, std::vector<Variable> parameters = std::vector<Variable>()) {
        auto functionIterator = functions_.find(functionName);
        if (functionIterator != functions_.end()) {
            Function* function = functionIterator->second;
            enterFunction(*function);
            Variable result = function->execute(*this);
            leaveFunction();
            return result;
        } else {
            throw NoFunctionWithName(functionName);
        }
    }


    Variable& variable(uint32_t index) {
        return stack.top().at(index);
    }

    void print(std::string s) {
        stdout_ += s;
    }

    std::string stdout() {
        return stdout_;
    }

};


#endif //WASMINT_ENVIRONMENT_H

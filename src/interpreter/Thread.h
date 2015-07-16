#ifndef WASMINT_THREAD_H
#define WASMINT_THREAD_H

#include <Variable.h>
#include <vector>
#include <stack>
#include <map>
#include <Module.h>
#include "Heap.h"

ExceptionMessage(StackLimitReached)
ExceptionMessage(IllegalUseageOfBreak)
ExceptionMessage(IllegalUseageOfContinue)

class RuntimeEnvironment;

class Thread {

    uint32_t stackLimit = 50;

    /**
     * The stack containing the local variables. The indices of the variables in each vector are equal to their
     * local indices as used by get_local and set_local.
     */
    std::stack<std::vector<Variable>> stack;

    /**
     * Pushes a new vector on the stack and creates variables with the given types.
     */
    void createLocals(std::vector<Type*> variableTypes) {
        if (stack.size() >= stackLimit)
            throw StackLimitReached(std::to_string(stack.size()));

        stack.push(std::vector<Variable>());
        for(Type* type : variableTypes) {
            stack.top().push_back(Variable(type));
        }
    }

    /**
     * Leave the last entered function
     */
    void leaveFunction() {
        stack.pop();
    }

    void enterFunction(Function& function);

    RuntimeEnvironment& env_;

public:
    Thread(RuntimeEnvironment& env);

    Variable callFunction(std::string functionName, std::vector<Variable> parameters = std::vector<Variable>());


    Variable& variable(uint32_t index) {
        return stack.top().at(index);
    }

    RuntimeEnvironment& runtimeEnvironment() {
        return env_;
    }
};


#endif //WASMINT_THREAD_H

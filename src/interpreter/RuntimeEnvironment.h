

#ifndef WASMINT_ENVIRONMENT_H
#define WASMINT_ENVIRONMENT_H


#include <Variable.h>
#include <vector>
#include <stack>
#include <Function.h>
#include <map>
#include <Module.h>
#include "Heap.h"

ExceptionMessage(NoFunctionWithName)
ExceptionMessage(NoGlobalWithName)
ExceptionMessage(StackLimitReached)
ExceptionMessage(IllegalUseageOfBreak)
ExceptionMessage(IllegalUseageOfContinue)

class CalledBreak;
class CalledContinue;

/**
 * Contains all variable values during the interpretation of a program.
 * TODO: It currently also hosts the stack, which should be changed as soon as things get multithreaded...
 */
class RuntimeEnvironment {

    /**
     * The stack containing the local variables. The indices of the variables in each vector are equal to their
     * local indices as used by get_local and set_local.
     */
    std::stack<std::vector<Variable>> stack;

    /**
     * All functions that are accessible with the currently loaded modules.
     * The keys are the function names.
     */
    std::map<std::string, Function*> functions_;

    /**
     * All globals that are accessible with the currently loaded modules.
     * The keys are the variable names.
     */
    std::map<std::string, Variable> globals_;

    /**
     * The current heap.
     */
    Heap heap_;

    uint32_t stackLimit = 50;

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
     * The stdout of this program. We currently just append to this string and then read it via stdou().
     */
    std::string stdout_;

    /**
     * Prepare the environment to enter the given function.
     */
    void enterFunction(Function& function) {
        // We push the new locals to the stack before entering.
        createLocals(function.locals());
    }

    /**
     * Leave the last entered function
     */
    void leaveFunction() {
        stack.pop();
    }

public:

    /**
     * Loads all functions of that module into the namespace.
     * TODO: Also load globals here
     */
    void useModule(Module& module) {
        std::vector<Function*> functions = module.functions();
        for(Function* function : functions) {
            functions_[function->name()] = function;
        }

        for(Global& global : module.globals()) {
            globals_[global.name()] = Variable(global.type());
        }

    }

    Variable callFunction(std::string functionName, std::vector<Variable> parameters = std::vector<Variable>());

    Heap& heap() {
        return heap_;
    }

    Variable& variable(uint32_t index) {
        return stack.top().at(index);
    }

    Variable& global(std::string name) {
        auto globalIterator = globals_.find(name);
        if (globalIterator != globals_.end()) {
           return globalIterator->second;
        } else {
            throw NoGlobalWithName(name);
        }
    }

    void print(std::string s) {
        stdout_ += s;
    }

    std::string stdout() {
        return stdout_;
    }

};


#endif //WASMINT_ENVIRONMENT_H

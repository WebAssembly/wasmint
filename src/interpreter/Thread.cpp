//
// Created by teemperor on 05.10.15.
//

#include <instructions/controlflow/Break.h>
#include "Thread.h"
#include "MachineState.h"
#include <Function.h>
#include <instructions/controlflow/Continue.h>

Heap &Thread::heap() {
    return machineState_->heap();
}

void Thread::enterFunction(Function& function) {
    // We push the new locals to the stack before entering.
    createLocals(function.locals());
}

void Thread::createLocals(std::vector<Type*> variableTypes) {
    if (stack.size() >= stackLimit)
        throw StackLimitReached(std::to_string(stack.size()));

    stack.push(std::vector<Variable>());
    for(Type* type : variableTypes) {
        stack.top().push_back(Variable(type));
    }
}

Variable Thread::callFunction(std::string functionName, std::vector<Variable> parameters) {
    Function& function = machineState_->function(functionName);
    enterFunction(function);
    for(uint32_t i = 0; i < parameters.size(); i++) {
        variable(i) = parameters.at(i);
    }

    Variable result;
    try {
        result = function.execute(*this);
    } catch(CalledBreak e) {
        throw IllegalUseageOfBreak(std::string("break was used outside of a loop in function ") + functionName);
    } catch(CalledContinue e) {
        throw IllegalUseageOfContinue(std::string("continue was used outside of a loop in function ") + functionName);
    }
    leaveFunction();
    return result;
}
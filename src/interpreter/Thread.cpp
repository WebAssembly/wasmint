#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include "Thread.h"
#include <Function.h>


Thread::Thread(RuntimeEnvironment& env) : env_(env) {
}

void Thread::enterFunction(Function& function) {
    if (stack.size() >= stackLimit)
        throw StackLimitReached(std::to_string(stack.size()));

    // We push the new locals to the stack before entering.

    stack.push(FunctionState(function));
}

Variable Thread::callFunction(std::string functionName, std::vector<Variable> parameters) {
    auto functionIterator = env_.functions().find(functionName);
    if (functionIterator != env_.functions().end()) {
        Function* function = functionIterator->second;
        enterFunction(*function);
        for(uint32_t i = 0; i < parameters.size(); i++) {
            variable(i) = parameters.at(i);
        }

        Variable result;
        try {
            result = function->execute(*this);
        } catch(CalledBreak e) {
            throw IllegalUseageOfBreak(std::string("break was used outside of a loop in function ") + functionName);
        } catch(CalledContinue e) {
            throw IllegalUseageOfContinue(std::string("continue was used outside of a loop in function ") + functionName);
        }
        leaveFunction();
        return result;
    } else {
        throw NoFunctionWithName(functionName);
    }
}

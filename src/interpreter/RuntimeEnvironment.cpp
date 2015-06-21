
#include "RuntimeEnvironment.h"
#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include <Function.h>

Variable RuntimeEnvironment::callFunction(std::string functionName, std::vector<Variable> parameters) {
    auto functionIterator = functions_.find(functionName);
    if (functionIterator != functions_.end()) {
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

void RuntimeEnvironment::enterFunction(Function& function) {
    // We push the new locals to the stack before entering.
    createLocals(function.locals());
}

void RuntimeEnvironment::useModule(Module& module) {
    std::vector<Function*> functions = module.functions();
    for(Function* function : functions) {
        functions_[function->name()] = function;
    }

    for(Global& global : module.globals()) {
        globals_[global.name()] = Variable(global.type());
    }

}
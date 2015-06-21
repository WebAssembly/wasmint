
#include "RuntimeEnvironment.h"
#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>

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
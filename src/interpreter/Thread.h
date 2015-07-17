#ifndef WASMINT_THREAD_H
#define WASMINT_THREAD_H

#include <Variable.h>
#include <vector>
#include <stack>
#include <map>
#include <Module.h>
#include <Function.h>
#include "Heap.h"
#include "FunctionState.h"
#include "InstructionState.h"

ExceptionMessage(StackLimitReached)
ExceptionMessage(IllegalUseageOfBreak)
ExceptionMessage(IllegalUseageOfContinue)
ExceptionMessage(ThreadNotRunning)

class RuntimeEnvironment;
class InstructionState;

class Thread {

    uint32_t stackLimit = 50;

    /**
     * The stack containing the states of all called functions. The most recently called
     * function state is always on top of the stack.
     */
    std::stack<FunctionState> stack;

    /**
     * Leave the last entered function
     */
    void leaveFunction() {
        stack.pop();
    }

    void enterFunction(Function& function);

    RuntimeEnvironment& env_;

    InstructionState* currentInstructionState = nullptr;

public:
    Thread(RuntimeEnvironment& env);

    Thread& startAtFunction(std::string functionName, std::vector<Variable> parameters = std::vector<Variable>());
    Instruction* callFunction(std::string functionName, std::vector<Variable> parameters = std::vector<Variable>());

    void step();

    void stepUntilFinished();

    InstructionState & getInstructionState();

    Variable& variable(uint32_t index) {
        return stack.top().variable(index);
    }

    RuntimeEnvironment& runtimeEnvironment() {
        return env_;
    }
};


#endif //WASMINT_THREAD_H

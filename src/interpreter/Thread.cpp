#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include "Thread.h"
#include <Function.h>
#include <assert.h>
#include "InstructionState.h"


Thread::Thread(RuntimeEnvironment& env) : env_(env) {
}

void Thread::enterFunction(Function& function) {
    if (stack.size() >= stackLimit)
        throw StackLimitReached(std::to_string(stack.size()));

    // We push the new locals to the stack before entering.

    stack.push(FunctionState(function));
}

Instruction* Thread::callFunction(std::string functionName, std::vector<Variable> parameters) {
    auto functionIterator = env_.functions().find(functionName);
    if (functionIterator != env_.functions().end()) {
        Function* function = functionIterator->second;
        enterFunction(*function);
        for(uint32_t i = 0; i < parameters.size(); i++) {
            variable(i) = parameters.at(i);
        }
        return function->mainInstruction();
    } else {
        throw NoFunctionWithName(functionName);
    }
}

void Thread::step() {
    if (currentInstructionState) {
        Signal s = currentInstructionState->step(*this);
        assert(s == Signal::None);
    }
}

void Thread::stepUntilFinished() {
    if (currentInstructionState) {
        while (!currentInstructionState->finished()) {
            step();
        }
    }
}

InstructionState &Thread::getInstructionState() {
    if (currentInstructionState)
        return currentInstructionState->getChildOrThis();
    throw ThreadNotRunning("Thread not started");
}

Thread& Thread::startAtFunction(std::string functionName, std::vector<Variable> parameters) {
    currentInstructionState = new InstructionState(callFunction(functionName, parameters));
    return *this;
}

Thread::~Thread() {
    if (currentInstructionState)
        delete currentInstructionState;
}
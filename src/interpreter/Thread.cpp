/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include "Thread.h"
#include <Function.h>
#include <assert.h>
#include "InstructionState.h"
#include "MachineState.h"


Thread::Thread(MachineState & env) : env_(env) {
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

void Thread::stepRoundRobin() {
    if (currentInstructionState) {
        for (uint32_t i = 0; i < weight_; i++) {
            if (currentInstructionState->finished()) {
                return;
            }
            step();
        }
    }
}
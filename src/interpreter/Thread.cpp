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

#include "Thread.h"
#include <Function.h>
#include <assert.h>
#include "InstructionState.h"
#include "MachineState.h"

namespace wasmint {

    thread_local Thread* currentThread_ = nullptr;

    Thread::Thread(MachineState &env) : env_(env) {
    }

    void Thread::enterFunction(wasm_module::Function& function) {
        if (stack.size() >= stackLimit)
            throw StackLimitReached(std::to_string(stack.size()));

        // We push the new locals to the stack before entering.

        stack.push(FunctionState(function));
    }

    wasm_module::Instruction* Thread::callFunction(
            const std::string& moduleName, const std::string& functionName,
            std::vector<wasm_module::Variable> parameters) {

        wasm_module::Function* func = nullptr;


        wasm_module::Module& module = env_.getModule(moduleName);

        if (module.hasImport(functionName)) {

            const wasm_module::ModuleImport& moduleImport = module.getImport(functionName);

            func = &env_.getFunction(moduleImport.module(), moduleImport.signature().name());
        } else {
            func = &env_.getFunction(moduleName, functionName);
        }

        enterFunction(*func);

        for (uint32_t i = 0; i < parameters.size(); i++) {
            variable(i) = parameters.at(i);
        }

        return func->mainInstruction();
    }

    void Thread::step() {
        currentThread_ = this;
        if (currentInstructionState) {
            Signal s = currentInstructionState->step(*this);

            if (s == Signal::AssertTrap) {
                throw AssertTrap("TODO"); //TODO
            } else if (s != Signal::None) {
                throw UnhandledSignal("TODO"); //TODO
            }
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

    Thread &Thread::startAtFunction(std::string moduleName, std::string functionName, std::vector<wasm_module::Variable> parameters) {
        currentInstructionState = new InstructionState(callFunction(moduleName, functionName, parameters));
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

    Heap &Thread::getHeap(const wasm_module::Module& module) {
        auto iter = heapsByModuleName_.find(module.name());
        if (iter != heapsByModuleName_.end()) {
            return heapsByModuleName_[module.name()];
        } else {
            return heapsByModuleName_[module.name()] = Heap(module.heapData());
        }
    }

    Heap &Thread::heap() {
        getHeap(stack.top().module());
    }
}
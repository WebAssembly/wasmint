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

namespace wasmint {

    ExceptionMessage(StackLimitReached)
    ExceptionMessage(IllegalUseageOfBreak)
    ExceptionMessage(IllegalUseageOfContinue)
    ExceptionMessage(ThreadNotRunning)
    ExceptionMessage(AssertTrap)
    ExceptionMessage(UnhandledSignal)
    ExceptionMessage(FailedToSetFloatingPointMode)

    class MachineState;
    class InstructionState;

    class Thread {

        uint32_t stackLimit = 50;

        /**
         * The stack containing the states of all called functions. The most recently called
         * function state is always on top of the stack.
         */
        std::stack<FunctionState> stack;


        void enterFunction(wasm_module::Function& function);

        MachineState & env_;

        InstructionState* currentInstructionState = nullptr;

        uint32_t weight_ = 1;

        std::map<std::string, Heap> heapsByModuleName_;

        void stepInternal();

    public:
        Thread(MachineState & env);
        virtual ~Thread();

        Thread& startAtFunction(std::string moduleName, std::string functionName, std::vector<wasm_module::Variable> parameters = std::vector<wasm_module::Variable>());

        wasm_module::Instruction* callFunction(const std::string& moduleName, const std::string& functionName, std::vector<wasm_module::Variable> parameters = std::vector<wasm_module::Variable>());

        std::size_t stackSize() {
            return stack.size();
        }

        /**
         * Leave the last entered function
         */
        void leaveFunction() {
            stack.pop();
        }

        void step();

        void stepUntilFinished();

        void stepRoundRobin();

        Heap&heap();

        Heap& getHeap(const wasm_module::Module& module);

        InstructionState & getInstructionState();

        wasm_module::Variable& variable(uint32_t index) {
            return stack.top().variable(index);
        }

        const std::vector<wasm_module::Variable>& locals() {
            return stack.top().variables();
        }

        MachineState & runtimeEnvironment() {
            return env_;
        }

        uint32_t weight() const {
            return weight_;
        }

        void weight(uint32_t weight) {
            this->weight_ = weight;
        }
    };

    extern thread_local Thread* currentThread_;
}

#endif //WASMINT_THREAD_H

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
#include <serialization/ByteOutputStream.h>
#include <serialization/Serializeable.h>
#include <serialization/ByteInputStream.h>
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

    class Thread : public Serializeable {

        uint32_t stackLimit = 50;

        /**
         * The stack containing the states of all called functions. The most recently called
         * function state is always on top of the stack.
         */
        std::vector<FunctionState> stack;

        MachineState& env_;

        InstructionState* currentInstructionState = nullptr;

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

        void setState(ByteInputStream& stream);

        bool finished() const;

        /**
         * Leave the last entered function
         */
        void leaveFunction() {
            if (stack.empty())
                throw std::domain_error("Can't leave function. Stack is empty!");
            stack.resize(stack.size() - 1);
        }

        void step();

        void stepUntilFinished();

        Heap&heap();

        Heap& getHeap(const wasm_module::Module& module);

        InstructionState & getInstructionState();

        wasm_module::Variable& variable(uint32_t index) {
            return stack.back().variable(index);
        }

        const std::vector<wasm_module::Variable>& locals() {
            return stack.back().variables();
        }

        MachineState & runtimeEnvironment() {
            return env_;
        }

        virtual void serialize(ByteOutputStream& stream) const override;
    };

    extern thread_local Thread* currentThread_;
}

#endif //WASMINT_THREAD_H

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
#include <interpreter/heap/Heap.h>
#include <interpreter/thread/FunctionState.h>
#include <interpreter/thread/InstructionState.h>

namespace wasmint {

    ExceptionMessage(StackLimitReached)
    ExceptionMessage(InstructionStackLimitReached)
    ExceptionMessage(IllegalUseageOfBreak)
    ExceptionMessage(IllegalUseageOfContinue)
    ExceptionMessage(AssertTrap)
    ExceptionMessage(UnhandledSignal)
    ExceptionMessage(FailedToSetFloatingPointMode)
    ExceptionMessage(InstructionStackIsEmpty)

    class MachineState;
    class InstructionState;

    class Thread : public Serializeable {

        friend class InstructionState;

        std::string trapReason_ = "Unknown reason";

        uint32_t functionStackLimit = 50000;
        uint32_t instructionStackLimit = 400000;

        /**
         * The functionStack_ containing the states of all called functions. The most recently called
         * function state is always on top of the functionStack_.
         */
        std::vector<FunctionState> functionStack_;
        std::vector<InstructionState> instructionStack_;

        MachineState& env_;

        std::map<std::string, Heap> heapsByModuleName_;

        void stepInternal();

    public:
        Thread(MachineState & env);
        virtual ~Thread();

        Thread& startAtFunction(const std::string& moduleName, const std::string& functionName, std::vector<wasm_module::Variable> parameters = std::vector<wasm_module::Variable>());

        wasm_module::Instruction* callFunction(const std::string& moduleName, const std::string& functionName, std::vector<wasm_module::Variable> parameters = std::vector<wasm_module::Variable>());

        std::size_t functionStackSize() {
            return functionStack_.size();
        }

        std::size_t instructionStackSize() {
            return instructionStack_.size();
        }

        void setState(ByteInputStream& stream);

        bool gotTrap() const;

        Signal trapReason(const std::string& reason) {
            trapReason_ = reason;
            return Signal::AssertTrap;
        }

        const std::string& trapReason() const {
            return trapReason_;
        }

        bool canStep() const;

        bool hasInstructionParent() {
            return instructionStack_.size() >= 2;
        }

        InstructionState& getInstructionParent() {
            return instructionStack_.at(instructionStack_.size() - 2);
        }

        bool hasCurrentInstruction() const {
            return !instructionStack_.empty();
        }

        InstructionState& getInstructionState() {
            if (!hasCurrentInstruction())
                throw InstructionStackIsEmpty("getInstructionState() can't be called on a empty stack");
            return instructionStack_.back();
        }

        const InstructionState& getInstructionState() const {
            if (!hasCurrentInstruction())
                throw InstructionStackIsEmpty("getInstructionState() can't be called on a empty stack");
            return instructionStack_.back();
        }

        void pushInstructionState(const wasm_module::Instruction& instruction);

        void popInstructionState();

        /**
         * Leave the last entered function
         */
        void leaveFunction() {
            if (functionStack_.empty())
                throw std::domain_error("Can't leave function. Stack is empty!");
            functionStack_.resize(functionStack_.size() - 1);
        }

        void step();

        void stepUntilFinished();

        Heap&heap();

        Heap& getHeap(const wasm_module::Module& module);

        wasm_module::Variable& variable(uint32_t index) {
            return functionStack_.back().variable(index);
        }

        const std::vector<wasm_module::Variable>& locals() {
            return functionStack_.back().variables();
        }

        MachineState & runtimeEnvironment() {
            return env_;
        }

        bool canIncreaseStack() const;

        virtual void serialize(ByteOutputStream& stream) const override;
    };

    extern thread_local Thread* currentThread_;
}

#endif //WASMINT_THREAD_H

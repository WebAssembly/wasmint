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

#include "InterpreterThread.h"
#include <iostream>
#include <fenv.h>
#include <interpreter/at/MachineState.h>

namespace wasmint {

    class FloatingPointGuard {
        int oldMode;
    public:
        FloatingPointGuard() {
            oldMode = fegetround();

            if (fesetround(FE_TONEAREST)) {
                throw FailedToSetFloatingPointMode("Failed to set floating point rounding mode to FE_TONEAREST");
            }
        }
        ~FloatingPointGuard() {
            if (fesetround(oldMode)) {
                throw FailedToSetFloatingPointMode("Failed to reset floating point rounding mode to its original value.");
            }
        }
    };

    thread_local InterpreterThread * currentThread_ = nullptr;

    InterpreterThread::InterpreterThread(MachineState &env) : env_(env) {
    }


    const wasm_module::Instruction* InterpreterThread::callFunction(
            const std::string& moduleName, const std::string& functionName,
            std::vector<wasm_module::Variable> parameters) {

        if (functionStack_.size() >= functionStackLimit)
            throw StackLimitReached(std::to_string(functionStack_.size()));

        const wasm_module::Function* func = nullptr;

        wasm_module::Module& module = env_.getModule(moduleName);

        func = &env_.getFunction(moduleName, functionName);

        if (func->variadic()) {
            std::vector<const wasm_module::Type*> types;
            for (auto& parameter : parameters) {
                types.push_back(&parameter.type());
            }
            functionStack_.push_back(FunctionState(*func, types));
        } else {
            // We push the new locals to the functionStack_ before entering.
            functionStack_.push_back(FunctionState(*func));
        }

        for (uint32_t i = 0; i < parameters.size(); i++) {
            variable(i) = parameters.at(i);
        }

        return func->mainInstruction();
    }

    void InterpreterThread::step() {
        FloatingPointGuard floatingPointGuard;
        if (!canStep())
            throw std::domain_error("Can't step");
        stepInternal();
    }

    void InterpreterThread::stepUntilFinished() {
        FloatingPointGuard floatingPointGuard;
        while (canStep()) {
            stepInternal();
        }
    }

    InterpreterThread& InterpreterThread::startAtFunction(const std::string& moduleName, const std::string& functionName, std::vector<wasm_module::Variable> parameters) {
        instructionStack_.push_back(InstructionState(*this, *callFunction(moduleName, functionName, parameters)));
        return *this;
    }

    InterpreterThread::~InterpreterThread() {
    }

    Heap &InterpreterThread::getHeap(const wasm_module::Module& module) {
        auto iter = heapsByModuleName_.find(module.name());
        if (iter != heapsByModuleName_.end()) {
            return heapsByModuleName_[module.name()];
        } else {
            return heapsByModuleName_[module.name()] = Heap(module.heapData());
        }
    }

    Heap &InterpreterThread::heap() {
        return getHeap(functionStack_.back().module());
    }

    void InterpreterThread::stepInternal() {
        currentThread_ = this;
        getInstructionState().step();
    }

    void InterpreterThread::serialize(ByteOutputStream& stream) const {
        stream.writeUInt32(functionStackLimit);

        stream.writeUInt64(functionStack_.size());
        for (const FunctionState& functionState : functionStack_) {
            functionState.serialize(stream);
        }

        stream.writeUInt64(instructionStack_.size());
        for (const InstructionState& instructionState : instructionStack_) {
            instructionState.serialize(stream);
        }

        stream.writeUInt64(heapsByModuleName_.size());
        for (auto& pair : heapsByModuleName_) {
            stream.writeStr(pair.first);
            pair.second.serialize(stream);
        }
    }

    void InterpreterThread::setState(ByteInputStream& stream) {
        functionStackLimit = stream.getUInt32();

        uint64_t stackSize = stream.getUInt64();

        for (std::uint64_t i = 0; i < stackSize; i++) {
            FunctionState functionState;
            functionState.setState(stream);
        }

        uint64_t instructionStackSize = stream.getUInt64();
        instructionStack_.resize(instructionStackSize);
        for (uint64_t i = 0; i < instructionStackSize; i++) {
            instructionStack_.at(i).setThread(*this);
            instructionStack_.at(i).setState(stream, env_);
        }

        uint64_t numberOfHeaps = stream.getUInt64();
        for (uint64_t i = 0; i < numberOfHeaps; i++) {
            std::string moduleName = stream.getString();
            Heap& heap = heapsByModuleName_[moduleName] = Heap();
            heap.setState(stream);
        }
    }

    bool InterpreterThread::gotTrap() const {
        if (!hasCurrentInstruction())
            return false;
        return getInstructionState().unhandledSignal();
    }

    bool InterpreterThread::canStep() const {
        if (!hasCurrentInstruction())
            return false;
        if (getInstructionState().unhandledSignal())
            return false;
        if (getInstructionState().finished())
            return false;
        return true;
    }

    bool InterpreterThread::canIncreaseStack() const {
        return functionStack_.size() < functionStackLimit;
    }

    void InterpreterThread::pushInstructionState(const wasm_module::Instruction& instruction) {
        if (instructionStack_.size() >= instructionStackLimit)
            throw InstructionStackLimitReached("Size of " + std::to_string(instructionStackLimit) + " is already reached");
        instructionStack_.push_back(InstructionState(*this, instruction));
    }

    void InterpreterThread::popInstructionState() {
        if (instructionStack_.empty())
            throw InstructionStackIsEmpty("popInstructionState() can't be called on a empty stack");
        instructionStack_.resize(instructionStack_.size() - 1);
    }
}
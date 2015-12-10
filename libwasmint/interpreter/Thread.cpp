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
#include <iostream>
#include <fenv.h>
#include "InstructionState.h"
#include "MachineState.h"

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

    thread_local Thread* currentThread_ = nullptr;

    Thread::Thread(MachineState &env) : env_(env) {
    }


    wasm_module::Instruction* Thread::callFunction(
            const std::string& moduleName, const std::string& functionName,
            std::vector<wasm_module::Variable> parameters) {

        if (stack.size() >= stackLimit)
            throw StackLimitReached(std::to_string(stack.size()));

        wasm_module::Function* func = nullptr;

        wasm_module::Module& module = env_.getModule(moduleName);

        func = &env_.getFunction(moduleName, functionName);

        if (func->variadic()) {
            std::vector<const wasm_module::Type*> types;
            for (auto& parameter : parameters) {
                types.push_back(&parameter.type());
            }
            stack.push_back(FunctionState(*func, types));
        } else {
            // We push the new locals to the stack before entering.
            stack.push_back(FunctionState(*func));
        }

        for (uint32_t i = 0; i < parameters.size(); i++) {
            variable(i) = parameters.at(i);
        }

        return func->mainInstruction();
    }

    void Thread::step() {
        FloatingPointGuard floatingPointGuard;
        if (!canStep())
            throw std::domain_error("Can't step");
        stepInternal();
    }

    void Thread::stepUntilFinished() {
        FloatingPointGuard floatingPointGuard;
        while (canStep()) {
            stepInternal();
        }
    }

    InstructionState &Thread::getRootInstructionState() {
        if (rootInstructionState_)
            return *rootInstructionState_;
        throw ThreadNotRunning("Thread has not started");
    }

    Thread& Thread::startAtFunction(const std::string& moduleName, const std::string& functionName, std::vector<wasm_module::Variable> parameters) {
        currentInstructionState_ = rootInstructionState_ = new InstructionState(*this, callFunction(moduleName, functionName, parameters));
        return *this;
    }

    Thread::~Thread() {
        if (rootInstructionState_)
            delete rootInstructionState_;
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
        return getHeap(stack.back().module());
    }

    void Thread::stepInternal() {
        currentThread_ = this;
        currentInstructionState_->step();
    }

    void Thread::serialize(ByteOutputStream& stream) const {
        stream.writeUInt32(stackLimit);

        stream.writeUInt64(stack.size());
        for (const FunctionState& functionState : stack) {
            functionState.serialize(stream);
        }

        if (rootInstructionState_) {
            stream.writeBool(true);
            rootInstructionState_->serialize(stream);
        } else {
            stream.writeBool(false);
        }

        stream.writeUInt64(heapsByModuleName_.size());
        for (auto& pair : heapsByModuleName_) {
            stream.writeStr(pair.first);
            pair.second.serialize(stream);
        }
    }

    void Thread::setState(ByteInputStream& stream) {
        stackLimit = stream.getUInt32();

        uint64_t stackSize = stream.getUInt64();

        for (std::uint64_t i = 0; i < stackSize; i++) {
            FunctionState functionState;
            functionState.setState(stream);
        }

        if (stream.getBool()) {
            rootInstructionState_ = new InstructionState(*this);
            rootInstructionState_->setState(stream, env_);
            currentInstructionState_ = &rootInstructionState_->getChildOrThis();
        }

        uint64_t numberOfHeaps = stream.getUInt64();
        for (uint64_t i = 0; i < numberOfHeaps; i++) {
            std::string moduleName = stream.getString();
            Heap& heap = heapsByModuleName_[moduleName] = Heap();
            heap.setState(stream);
        }
    }

    void Thread::setCurrentInstructionState(InstructionState* newState) {
        currentInstructionState_ = newState;
    }

    bool Thread::gotTrap() const {
        if (!currentInstructionState_)
            return false;
        return currentInstructionState_->unhandledSignal();
    }

    bool Thread::canStep() const {
        if (currentInstructionState_ == nullptr)
            return false;
        if (currentInstructionState_->unhandledSignal())
            return false;
        if (currentInstructionState_->finished())
            return false;
        return true;
    }

    InstructionState &Thread::getCurrentInstructionState() {
        return *currentInstructionState_;
    }

    bool Thread::canIncreaseStack() const {
        return stack.size() < stackLimit;
    }
}
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

#include "InstructionState.h"

#include <instructions/Instruction.h>
#include "Thread.h"
#include "InstructionExecutor.h"
#include <types/Type.h>
#include <iostream>
#include <interpreter/MachineState.h>

namespace wasmint {

    void InstructionState::step() {
        if (unhandledSignal_)
            throw std::domain_error("Cant step in a InstructionState with a unhandled signal");
        if (finished_)
            throw std::domain_error("Cant step in a finished InstructionState");

        StepResult result = InstructionExecutor::execute(*instruction(), *thread_);
        state_++;

        if (result.newChildInstruction()) {
            childInstruction = new InstructionState(*thread_, result.newChildInstruction(), this);
        } else if (result.signal() == Signal::Return || result.signal() == Signal::Branch) {
            if (parent_) {
                parent_->finishSignal(result);
            } else {
                unhandledSignal_ = true;
            }
        } else if (result.signal() == Signal::AssertTrap) {
            unhandledSignal_ = true;
        } else {
            if (!wasm_module::Type::typeCompatible(instruction()->returnType(), &result.result().type())) {
                throw IncompatibleChildReturnType(instruction()->name() + " is supposed to return "
                                                  + instruction()->returnType()->name() + " but returned "
                                                  + result.result().type().name());
            }
            finished_ = true;
            if (parent_)
                parent_->finishSignal(result);
        }
    }

    void InstructionState::finishSignal(StepResult result) {
        results_.push_back(result.result());
        delete childInstruction;
        childInstruction = nullptr;
        thread_->setCurrentInstructionState(this);

        if (result.signal() == Signal::Branch) {
            if (!InstructionExecutor::handleSignal(*instruction(), *this, result)) {
                if (parent_)
                    parent_->finishSignal(result);
                else
                    unhandledSignal_ = true;
            }
        } else if (result.signal() == Signal::Return) {
            if (instruction()->hasParent()) {
                if (parent_)
                    parent_->finishSignal(result);
                else
                    throw std::domain_error("InstructionState has no parent but related Instruction has a parent.");
            } else {
                if (parent_)
                    parent_->finishSignal(result.result());
            }
        }
    }

    InstructionState::~InstructionState() {
        if (childInstruction)
            delete childInstruction;
    }

    InstructionState::InstructionState(Thread& thread, wasm_module::Instruction *instruction, InstructionState* parent)
            : instruction_(instruction), parent_(parent), thread_(&thread) {
        thread_->setCurrentInstructionState(this);
    }

    void InstructionState::serialize(ByteOutputStream& stream) const {
        stream.writeUInt32(state_);
        stream.writeBool(finished_);
        stream.writeBool(unhandledSignal_);

        stream.writeUInt64(results_.size());
        for (const wasm_module::Variable& var : results_) {
            stream.writeVariable(var);
        }

        stream.writeInstructionAddress(instruction_->getAddress());
        stream.writeVariable(branchValue_);
        stream.writeBool(hasBranchValue_);

        if (childInstruction) {
            stream.writeBool(true);
            childInstruction->serialize(stream);
        } else {
            stream.writeBool(false);
        }
    }

    void InstructionState::setState(ByteInputStream& stream, MachineState& state) {
        state_ = stream.getUInt32();
        finished_ = stream.getBool();
        unhandledSignal_ = stream.getBool();

        results_.clear();
        uint64_t resultsSize = stream.getUInt64();
        for (uint64_t i = 0; i < resultsSize; i++) {
            results_.push_back(stream.getVariable());
        }

        wasm_module::InstructionAddress address = stream.getInstructionAddress();
        instruction_ = state.getInstruction(address);

        branchValue_ = stream.getVariable();
        hasBranchValue_ = stream.getBool();

        if (stream.getBool()) {
            childInstruction = new InstructionState(*this);
            childInstruction->setState(stream, state);
        }
    }

}
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

    StepResult InstructionState::step(Thread &thread) {
        if (childInstruction != nullptr) {
            StepResult stepResult = childInstruction->step(thread);
            if (childInstruction->finished()) {
                results_.push_back(childInstruction->result());
                delete childInstruction;
                childInstruction = nullptr;
            }

            if (stepResult.signal() == Signal::Branch) {
                if (InstructionExecutor::handleSignal(*instruction(), *this, stepResult)) {
                    delete childInstruction;
                    childInstruction = nullptr;
                    return Signal::None;
                }
            }
            if (stepResult.signal() == Signal::Return) {
                if (!instruction()->hasParent()) {
                    result_ = stepResult.result();
                    finished_ = true;
                    return Signal::None;
                }
            }

            return stepResult;
        } else {
            StepResult result = InstructionExecutor::execute(*instruction(), thread);
            if (result.newChildInstruction()) {
                childInstruction = new InstructionState(*thread_, result.newChildInstruction(), this);
                //TODO std::cout << "Entering " << result.newChildInstruction()->toSExprString() << std::endl;
            } else if (result.signal() != Signal::None) {
                //TODO std::cout << "Got Signal from " << instruction()->dataString() << std::endl;
                return result;
            } else {
                result_ = result.result();
                if (!wasm_module::Type::typeCompatible(instruction()->returnType(), &result_.type())) {
                    throw IncompatibleChildReturnType(instruction()->name() + " is supposed to return "
                                                      + instruction()->returnType()->name() + " but returned "
                                                      + result_.type().name());
                }
                //TODO std::cout << instruction()->toSExprString() << " : " << result_.toString() << std::endl;
                finished_ = true;
            }
            state_++;
        }
        return Signal::None;
    }

    InstructionState::~InstructionState() {
        if (childInstruction)
            delete childInstruction;
    }

    InstructionState::InstructionState(Thread& thread, wasm_module::Instruction *instruction, InstructionState* parent)
            : instruction_(instruction), parent_(parent), thread_(&thread) {
    }

    void InstructionState::serialize(ByteOutputStream& stream) const {
        stream.writeUInt32(state_);
        stream.writeVariable(result_);
        stream.writeBool(finished_);

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
        result_ = stream.getVariable();
        finished_ = stream.getBool();

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
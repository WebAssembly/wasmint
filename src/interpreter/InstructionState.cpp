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
                childInstruction = new InstructionState(result.newChildInstruction(), this);
                // TODO std::cout << "Entering " << result.newChildInstruction()->toSExprString() << std::endl;
            } else if (result.signal() != Signal::None) {
                // TODO std::cout << "Got Signal from " << instruction()->dataString() << std::endl;
                return result;
            } else {
                result_ = result.result();
                if (!wasm_module::Type::typeCompatible(instruction()->returnType(), &result_.type())) {
                    throw IncompatibleChildReturnType(instruction()->name() + " is supposed to return "
                                                      + instruction()->returnType()->name() + " but returned "
                                                      + result_.type().name());
                }
                // TODO std::cout << instruction()->toSExprString() << " : " << result_.toString() << std::endl;
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

    InstructionState::InstructionState(wasm_module::Instruction *instruction, InstructionState* parent)
            : instruction_(instruction), parent_(parent) {
    }
}
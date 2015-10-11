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

#include "InstructionExecutor.h"
#include <instructions/controlflow/Block.h>
#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include <instructions/controlflow/DoWhile.h>
#include <instructions/controlflow/Forever.h>
#include <instructions/controlflow/If.h>
#include <instructions/controlflow/Return.h>

#include <instructions/heap/Int32Load.h>
#include <instructions/heap/Int32Store.h>

#include <instructions/I32/I32Add.h>
#include <instructions/I32/I32Div.h>
#include <instructions/I32/I32Mul.h>
#include <instructions/I32/I32Sub.h>

#include <instructions/FunctionCall.h>
#include <instructions/GetGlobal.h>
#include <instructions/GetLocal.h>
#include <instructions/Literal.h>
#include <instructions/Print.h>
#include <instructions/SetGlobal.h>
#include <instructions/SetLocal.h>
#include <instructions/NativeInstruction.h>
#include <instructions/assert/I32AssertReturn.h>

#include "MachineState.h"

namespace wasmint {

    StepResult InstructionExecutor::execute(wasm_module::Instruction &instruction, Thread &thread) {
        InstructionState &state = thread.getInstructionState();

        /**
         * Control flow
         */
        if (typeid(instruction) == typeid(wasm_module::Block)) {
            if (state.state() < instruction.children().size()) {
                return StepResult(instruction.children().at(state.state()));
            } else {
                return StepResult();
            }
        } else if (typeid(instruction) == typeid(wasm_module::Break)) {
            return StepResult(Signal::Break);
        } else if (typeid(instruction) == typeid(wasm_module::Continue)) {
            return StepResult(Signal::Continue);
        } else if (typeid(instruction) == typeid(wasm_module::DoWhile)) {
            switch (state.state()) {
                case 0:
                case 1:
                    return instruction.children().at(0);
                case 2:
                    return instruction.children().at(1);
                default:
                    if (wasm_module::Int32::getValue(state.results().back()) != 0) {
                        state.state(0);
                    } else {
                        return StepResult();
                    }
            }
        } else if (typeid(instruction) == typeid(wasm_module::Forever)) {
            if (thread.getInstructionState().state() >= 10) {
                return StepResult();
            }
            thread.getInstructionState().clearResults();
            thread.getInstructionState().state(0);
            return StepResult(instruction.children().front());
        } else if (typeid(instruction) == typeid(wasm_module::If)) {
            switch (state.state()) {
                case 0:
                    return instruction.children().at(0);
                case 1:
                    if (wasm_module::Int32::getValue(state.results().front()) != 0) {
                        return instruction.children().at(1);
                    }
                default:
                    return StepResult();

            }
        } else if (typeid(instruction) == typeid(wasm_module::Return)) {
            switch (state.state()) {
                case 0:
                    return instruction.children().at(0);
                default:
                    return state.results().front();

            }
        }
        /**
         * heap access
         */
        else if (typeid(instruction) == typeid(wasm_module::Int32Load)) {
            switch (state.state()) {
                case 0:
                    return StepResult(instruction.children().at(0));
                default:
                    uint32_t offset = static_cast<uint32_t>(wasm_module::Int32::getValue(state.results().back()));

                    std::vector<uint8_t> bytes = thread.runtimeEnvironment().heap().getBytes(offset,
                                                                                             wasm_module::Int32::instance()->size());

                    wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance()->localType());
                    result.setValue(bytes);
                    return result;
            }
        } else if (typeid(instruction) == typeid(wasm_module::Int32Store)) {
            switch (state.state()) {
                case 0:
                    return StepResult(instruction.children().at(0));
                case 1:
                    return StepResult(instruction.children().at(1));
                default:

                    uint32_t offset = static_cast<uint32_t>(wasm_module::Int32::getValue(state.results().at(0)));

                    wasm_module::Variable value = state.results().at(1);
                    thread.runtimeEnvironment().heap().setBytes(offset, value.data());
                    return StepResult(value);
            }
        }
        /**
         * i32 instructions
         */
        else if (typeid(instruction) == typeid(wasm_module::I32Add)) {
            switch (state.state()) {
                case 0:
                    return StepResult(instruction.children().at(0));
                case 1:
                    return StepResult(instruction.children().at(1));
                default:
                    int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                    int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                    wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                    wasm_module::Int32::setValue(result, left + right);
                    return result;
            }
        } else if (typeid(instruction) == typeid(wasm_module::I32Div)) {
            switch (state.state()) {
                case 0:
                    return StepResult(instruction.children().at(0));
                case 1:
                    return StepResult(instruction.children().at(1));
                default:
                    int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                    int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                    // TODO that exception should be in the interpreter namespace
                    if (right == 0)
                        throw wasm_module::DivisionThroughZero();

                    wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                    wasm_module::Int32::setValue(result, left + right);
                    return result;
            }
        } else if (typeid(instruction) == typeid(wasm_module::I32Mul)) {
            switch (state.state()) {
                case 0:
                    return StepResult(instruction.children().at(0));
                case 1:
                    return StepResult(instruction.children().at(1));
                default:
                    int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                    int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                    wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                    wasm_module::Int32::setValue(result, left * right);
                    return result;
            }
        } else if (typeid(instruction) == typeid(wasm_module::I32Sub)) {
            switch (state.state()) {
                case 0:
                    return StepResult(instruction.children().at(0));
                case 1:
                    return StepResult(instruction.children().at(1));
                default:
                    int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                    int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                    wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                    wasm_module::Int32::setValue(result, left - right);
                    return result;
            }
        }
        /**
         * other instructions
         */
        else if (typeid(instruction) == typeid(wasm_module::I32AssertReturn)) {
            switch (state.state()) {
                case 0:
                    return StepResult(instruction.children().at(0));
                case 1:
                    return StepResult(instruction.children().at(1));
                default:
                    int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                    int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                    if (left != right) {
                        return StepResult(Signal::AssertTrap);
                    } else {
                        return StepResult(Signal::None);
                    }
            }
        } else if (typeid(instruction) == typeid(wasm_module::FunctionCall)) {
            if (state.state() < instruction.children().size()) {
                return StepResult(instruction.children().at(state.state()));
            } else if (state.state() == instruction.children().size()) {
                std::vector<wasm_module::Variable> parameters;
                for (uint32_t i = 0; i < state.results().size(); i++) {
                    parameters.push_back(state.results().at(i));
                }
                wasm_module::FunctionCall& functionCall = dynamic_cast<wasm_module::FunctionCall &>(instruction);
                return StepResult(thread.callFunction(functionCall.moduleName, functionCall.functionSignature.name(), parameters));
            } else {
                thread.leaveFunction();
                return state.results().back();
            }
        } else if (typeid(instruction) == typeid(wasm_module::GetGlobal)) {
            return thread.runtimeEnvironment().global(dynamic_cast<wasm_module::GetGlobal &>(instruction).globalName);
        } else if (typeid(instruction) == typeid(wasm_module::NativeInstruction)) {
            return StepResult(dynamic_cast<wasm_module::NativeInstruction &>(instruction).call(thread.locals()));
        } else if (typeid(instruction) == typeid(wasm_module::GetLocal)) {
            return thread.variable(dynamic_cast<wasm_module::GetLocal &>(instruction).localIndex);
        } else if (typeid(instruction) == typeid(wasm_module::Literal)) {
            return StepResult(dynamic_cast<wasm_module::Literal &>(instruction).literalValue());
        } else if (typeid(instruction) == typeid(wasm_module::Print)) {
            switch (state.state()) {
                case 0:
                    return instruction.children().at(0);
                default:
                    thread.runtimeEnvironment().print(std::to_string(wasm_module::Int32::getValue(state.results().at(0))));
                    return StepResult();
            }
        } else if (typeid(instruction) == typeid(wasm_module::SetGlobal)) {
            switch (state.state()) {
                case 0:
                    return instruction.children().at(0);
                default:
                    wasm_module::Variable result = thread.runtimeEnvironment().global(
                            dynamic_cast<wasm_module::SetGlobal &>(instruction).globalName) = state.results().at(0);
                    return result;

            }
        } else if (typeid(instruction) == typeid(wasm_module::SetLocal)) {
            switch (state.state()) {
                case 0:
                    return instruction.children().at(0);
                default:
                    wasm_module::Variable result = thread.variable(
                            dynamic_cast<wasm_module::SetLocal &>(instruction).localIndex) = state.results().at(0);
                    return result;
            }
        }
        else {
            throw UnknownInstruction(std::string("Pointer points to an unkown Instruction type"));
        }
    }

    bool InstructionExecutor::handleSignal(wasm_module::Instruction &instruction, InstructionState &currentState, Signal signal) {

        if (typeid(instruction) == typeid(wasm_module::Forever) || typeid(instruction) == typeid(wasm_module::DoWhile)) {
            if (signal == Signal::Break) {
                currentState.state(10);
                return true;
            }
            return signal == Signal::Continue;
        }

        return false;
    }

}
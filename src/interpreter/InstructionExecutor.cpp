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

#include <instructions/assert/I32AssertReturn.h>
#include <instructions/Print.h>
#include <instructions/Instructions.h>

#include "MachineState.h"

namespace wasmint {

    StepResult InstructionExecutor::execute(wasm_module::Instruction &instruction, Thread &thread) {
        InstructionState &state = thread.getInstructionState();



        switch (instruction.id()) {
            case InstructionId::Block:
                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else {
                    return StepResult();
                }
            case InstructionId::Break:
                return StepResult(Signal::Break);
            case InstructionId::Continue:
                return StepResult(Signal::Continue);
            case InstructionId::DoWhile:
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
            case InstructionId::Forever:
                if (thread.getInstructionState().state() >= 10) {
                    return StepResult();
                }
                thread.getInstructionState().clearResults();
                thread.getInstructionState().state(0);
                return StepResult(instruction.children().front());
            case InstructionId::If:
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
            case InstructionId::Return:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    default:
                        return state.results().front();

                }
            case InstructionId::I32Add:
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
            case InstructionId::I32DivSigned:
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
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (uint32_t)(left / right);
                        return result;
                }
            case InstructionId::I32DivUnsigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    case 1:
                        return StepResult(instruction.children().at(1));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (uint32_t)(left / right);
                        return result;
                }
            case InstructionId::I32And:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    case 1:
                        return StepResult(instruction.children().at(1));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        wasm_module::Variable result = (uint32_t)(left & right);
                        return result;
                }
            case InstructionId::I32Mul:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    case 1:
                        return StepResult(instruction.children().at(1));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        wasm_module::Variable result = (int32_t)(left * right);
                        return result;
                }
            case InstructionId::I32Sub:
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
            case InstructionId::I32LessThanSigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    case 1:
                        return StepResult(instruction.children().at(1));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        if (left < right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }
            case InstructionId::I32LessEqualSigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    case 1:
                        return StepResult(instruction.children().at(1));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        if (left <= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }
            case InstructionId::I32AssertReturn:
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
            case InstructionId::Call:
                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else if (state.state() == instruction.children().size()) {
                    std::vector<wasm_module::Variable> parameters;
                    for (uint32_t i = 0; i < state.results().size(); i++) {
                        parameters.push_back(state.results().at(i));
                    }
                    wasm_module::Call & functionCall = dynamic_cast<wasm_module::Call &>(instruction);
                    return StepResult(thread.callFunction(functionCall.moduleName, functionCall.functionSignature.name(), parameters));
                } else {
                    thread.leaveFunction();
                    return state.results().back();
                }
            case InstructionId::NativeInstruction:
                return StepResult(dynamic_cast<wasm_module::NativeInstruction &>(instruction).call(thread.locals()));
            case InstructionId::GetLocal:
                return thread.variable(dynamic_cast<wasm_module::GetLocal &>(instruction).localIndex);
            case InstructionId::I32Const:
            case InstructionId::I64Const:
            case InstructionId::F32Const:
            case InstructionId::F64Const:
                return StepResult(dynamic_cast<wasm_module::Literal &>(instruction).literalValue());
            case InstructionId::Print:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    default:
                        thread.runtimeEnvironment().print(std::to_string(wasm_module::Int32::getValue(state.results().at(0))));
                        return StepResult();
                }
            case InstructionId::SetLocal:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    default:
                        wasm_module::Variable result = thread.variable(
                                dynamic_cast<wasm_module::SetLocal &>(instruction).localIndex) = state.results().at(0);
                        return result;
                }
            default:
                throw UnknownInstruction(std::string("Unknown Instruction " + instruction.name()));
        }

        /**
             * heap access
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
             */

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
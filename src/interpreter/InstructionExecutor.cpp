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

#include <instructions/Instructions.h>
#include <iostream>
#include <cmath>

#include "MachineState.h"

namespace wasmint {

    StepResult InstructionExecutor::execute(wasm_module::Instruction &instruction, Thread &thread) {
        using namespace wasm_module;

        InstructionState &state = thread.getInstructionState();

        switch (instruction.id()) {

            /******************************************************
             ***************** Int 32 Operations ******************
             ******************************************************/


            case InstructionId::I32Add:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = state.results().at(0).uint32();
                        uint32_t right = state.results().at(1).uint32();
                        uint32_t resultValue = left + right;

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setUnsignedValue(result, resultValue);
                        return result;
                }
            case InstructionId::I32Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left - right);
                        return result;
                }
            case InstructionId::I32Mul:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        wasm_module::Variable result = (int32_t)(left * right);
                        return result;
                }
            case InstructionId::I32DivSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (int32_t)(left / right);
                        return result;
                }
            case InstructionId::I32DivUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (uint32_t)(left / right);
                        return result;
                }
            case InstructionId::I32RemainderSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));
                        if (right < 0)
                            right = -right;

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (int32_t)(left % right);
                        return result;
                }
            case InstructionId::I32RemainderUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (uint32_t)(left % right);
                        return result;
                }
            case InstructionId::I32And:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        wasm_module::Variable result = (uint32_t)(left & right);
                        return result;
                }

            case InstructionId::I32Or:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        wasm_module::Variable result = (uint32_t)(left | right);
                        return result;
                }

            case InstructionId::I32Xor:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        wasm_module::Variable result = (uint32_t)(left ^ right);
                        return result;
                }

            case InstructionId::I32ShiftLeft:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (right >= 32u) {
                            // that would be unspecified in C, so we do it manually
                            return wasm_module::Variable((uint32_t) 0);
                        }

                        wasm_module::Variable result = (uint32_t)(left << right);
                        return result;
                }

            case InstructionId::I32ShiftRightZeroes:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (right >= 32u) {
                            // that would be unspecified in C, so we do it manually
                            return wasm_module::Variable((uint32_t) 0);
                        }

                        wasm_module::Variable result = (uint32_t)(left >> right);
                        return result;
                }

            case InstructionId::I32ShiftRightSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        int32_t leftSigned = wasm_module::Int32::getValue(state.results().at(0));


                        if (right >= 32u) {
                            // that would be unspecified in C, so we do it manually
                            if (leftSigned < 0) {
                                return wasm_module::Variable((uint32_t) std::numeric_limits<uint32_t>::max());
                            } else {
                                return wasm_module::Variable((uint32_t) 0);
                            }
                        }

                        uint32_t resultInt = left >> right;

                        if (leftSigned < 0) {
                            if ((left & (0x1u << 31u)) != 0) {
                                uint32_t bitMask = 0;
                                for (uint32_t i = 0; i < right; i++) {
                                    bitMask >>= 1u;
                                    bitMask |= (0x1u << 31u);
                                }
                                resultInt |= bitMask;
                            }
                        }

                        wasm_module::Variable result = (uint32_t)(resultInt);
                        return result;
                }

            case InstructionId::I32Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (left == right) {
                            wasm_module::Variable result = (uint32_t)(1);
                            return result;
                        } else {
                            wasm_module::Variable result = (uint32_t)(0);
                            return result;
                        }
                }

            case InstructionId::I32NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (left != right) {
                            wasm_module::Variable result = (uint32_t)(1);
                            return result;
                        } else {
                            wasm_module::Variable result = (uint32_t)(0);
                            return result;
                        }
                }

            case InstructionId::I32LessThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
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
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        if (left <= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I32LessThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (left < right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I32LessEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (left <= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I32GreaterThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        if (left > right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I32GreaterEqualSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        if (left >= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I32GreaterThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (left > right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I32GreaterEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        if (left >= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }


            case InstructionId::I32CountLeadingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        uint32_t leadingZeroes = 0;

                        if (value == 0) {
                            return wasm_module::Variable((uint32_t) 32);
                        }

                        while (true) {
                            if ((value & (0x1u << 31u)) == 0) {
                                leadingZeroes++;
                                value <<= 1;
                            } else {
                                break;
                            }
                            if (value == 0)
                                break;
                        }

                        return wasm_module::Variable((uint32_t) leadingZeroes);
                }
            case InstructionId::I32CountTrailingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        uint32_t trailingZeroes = 0;

                        if (value == 0) {
                            return wasm_module::Variable((uint32_t) 32);
                        }

                        while (true) {
                            if ((value & 0x1u) == 0) {
                                trailingZeroes++;
                                value >>= 1;
                            } else {
                                break;
                            }
                            if (value == 0)
                                break;
                        }

                        return wasm_module::Variable((uint32_t) trailingZeroes);
                }

            case InstructionId::I32PopulationCount:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        uint32_t population = 0;

                        while (true) {
                            if ((value & 0x1u) == 1) {
                                population++;
                            }
                            value >>= 1;
                            if (value == 0)
                                break;
                        }

                        return wasm_module::Variable((uint32_t) population);
                }


            /******************************************************
             ***************** Int 64 Operations ******************
             ******************************************************/

            case InstructionId::I64Add:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance());
                        wasm_module::Int64::setValue(result, left + right);
                        return result;
                }
            case InstructionId::I64Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance());
                        wasm_module::Int64::setValue(result, left - right);
                        return result;
                }
            case InstructionId::I64Mul:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        wasm_module::Variable result = (int64_t)(left * right);
                        return result;
                }
            case InstructionId::I64DivSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (int64_t)(left / right);
                        return result;
                }
            case InstructionId::I64DivUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (uint64_t)(left / right);
                        return result;
                }
            case InstructionId::I64RemainderSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (int64_t)(left % right);
                        return result;
                }
            case InstructionId::I64RemainderUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        // TODO that exception should be in the interpreter namespace
                        if (right == 0)
                            throw DivisionThroughZero(std::to_string(left) + "/" + std::to_string(right));

                        wasm_module::Variable result = (uint64_t)(left / right);
                        return result;
                }
            case InstructionId::I64And:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        wasm_module::Variable result = (uint64_t)(left & right);
                        return result;
                }

            case InstructionId::I64Or:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        wasm_module::Variable result = (uint64_t)(left | right);
                        return result;
                }

            case InstructionId::I64Xor:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        wasm_module::Variable result = (uint64_t)(left ^ right);
                        return result;
                }

            case InstructionId::I64ShiftLeft:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (right >= 64u) {
                            // that would be unspecified in C, so we do it manually
                            return wasm_module::Variable((uint64_t) 0);
                        }

                        wasm_module::Variable result = (uint64_t)(left << right);
                        return result;
                }

            case InstructionId::I64ShiftRightZeroes:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (right >= 64u) {
                            // that would be unspecified in C, so we do it manually
                            return wasm_module::Variable((uint64_t) 0);
                        }

                        wasm_module::Variable result = (uint64_t)(left >> right);
                        return result;
                }

            case InstructionId::I64ShiftRightSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (right >= 64u) {
                            // that would be unspecified in C, so we do it manually
                            return wasm_module::Variable((uint64_t) 0);
                        }

                        uint64_t resultInt = left >> right;

                        if ((left & (0x1ul << 63u)) != 0) {
                            uint64_t bitMask = 0;
                            for(uint64_t i = 0; i < right; i++) {
                                bitMask >>= 1u;
                                bitMask |= (0x1ul << 63u);
                            }
                            resultInt |= bitMask;
                        }

                        wasm_module::Variable result = (uint64_t)(resultInt);
                        return result;
                }

            case InstructionId::I64Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (left == right) {
                            wasm_module::Variable result = (uint32_t)(1);
                            return result;
                        } else {
                            wasm_module::Variable result = (uint32_t)(0);
                            return result;
                        }
                }

            case InstructionId::I64NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (left != right) {
                            wasm_module::Variable result = (uint32_t)(1);
                            return result;
                        } else {
                            wasm_module::Variable result = (uint32_t)(0);
                            return result;
                        }
                }

            case InstructionId::I64LessThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        if (left < right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I64LessEqualSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        if (left <= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I64LessThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (left < right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I64LessEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (left <= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I64GreaterThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        if (left > right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I64GreaterEqualSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        if (left >= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I64GreaterThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (left > right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }

            case InstructionId::I64GreaterEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        if (left >= right) {
                            return wasm_module::Variable((int32_t) 1);
                        } else {
                            return wasm_module::Variable((int32_t) 0);
                        }
                }


            case InstructionId::I64CountLeadingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        uint32_t leadingZeroes = 0;

                        if (value == 0) {
                            return wasm_module::Variable((uint64_t) 64);
                        }

                        while (true) {
                            if ((value & (0x1ul << 63ul)) == 0) {
                                leadingZeroes++;
                                value <<= 1;
                            } else {
                                break;
                            }
                            if (value == 0)
                                break;
                        }

                        return wasm_module::Variable((uint64_t) leadingZeroes);
                }
            case InstructionId::I64CountTrailingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        uint64_t trailingZeroes = 0;

                        if (value == 0) {
                            return wasm_module::Variable((uint64_t) 64);
                        }

                        while (true) {
                            if ((value & 0x1u) == 0) {
                                trailingZeroes++;
                                value >>= 1;
                            } else {
                                break;
                            }
                            if (value == 0)
                                break;
                        }

                        return wasm_module::Variable((uint64_t) trailingZeroes);
                }

            case InstructionId::I64PopulationCount:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        uint64_t population = 0;

                        while (true) {
                            if ((value & 0x1u) == 1) {
                                population++;
                            }
                            value >>= 1;
                            if (value == 0)
                                break;
                        }

                        return wasm_module::Variable((uint64_t) population);
                }

            /******************************************************
             ************** Control Flow Operations ***************
             ******************************************************/

            case InstructionId::Comma:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        return state.results().back();
                }
            case InstructionId::Conditional:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    case 1:
                        {
                            Variable condition = state.results().front();
                            if (Int32::getUnsignedValue(condition) == 0) {
                                return instruction.children().at(2);
                            } else {
                                return instruction.children().at(1);
                            }
                        }
                    default:
                        return state.results().back();
                }

            case InstructionId::Block:
                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else {
                    if (state.results().empty())
                        return StepResult();
                    return StepResult(state.results().back());
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
                        } else {
                            return StepResult();
                        }
                    default:
                        return state.results().back();

                }
            case InstructionId::IfElse:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    case 1:
                        if (wasm_module::Int32::getValue(state.results().front()) != 0) {
                            return instruction.children().at(1);
                        } else {
                            return instruction.children().at(2);
                        }
                    default:
                        return state.results().back();

                }
            case InstructionId::Return:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    default:
                        return state.results().front();

                }

            case InstructionId::I32AssertReturn:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        if (left != right) {
                            // TODO don't print this to stdout
                            std::cout << "Trap: " << left << " != " << right << std::endl;
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
            case InstructionId::CallImport:
                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else if (state.state() == instruction.children().size()) {
                    std::vector<wasm_module::Variable> parameters;
                    for (uint32_t i = 0; i < state.results().size(); i++) {
                        parameters.push_back(state.results().at(i));
                    }
                    wasm_module::CallImport & functionCall = dynamic_cast<wasm_module::CallImport &>(instruction);
                    return StepResult(thread.callFunction(functionCall.functionSignature.name(), functionCall.functionSignature.name(), parameters));
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
            case InstructionId::HasFeature:
                {
                    Variable result(Int32::instance());
                    if (dynamic_cast<wasm_module::HasFeature &>(instruction).featureName() == "wasm") {
                        result.uint32(1);
                    } else {
                        result.uint32(0);
                    }
                    return result;
                }
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

            case InstructionId::GrowMemory:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        // TODO risky conversion
                        thread.heap().grow((uint32_t) value);

                        Variable result;
                        return StepResult(result);
                }

            case InstructionId::PageSize:
                {
                    Variable result = (uint64_t) 4096u;
                    return StepResult(result);
                }

            /******************************************************
             ************** Load / Store Operations ***************
             ******************************************************/
            case InstructionId::I32Load8Signed:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 1u);

                        std::vector<uint8_t> bytes = {loadedBytes.at(0), 0x0, 0x0, 0x0};

                        // sign extend if the loaded byte is negative
                        if (loadedBytes.at(0) > 127) {
                            bytes[1] = bytes[2] = bytes[3] = 0xFFu;
                        }

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I32Load8Unsigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 1u);
                        std::vector<uint8_t> bytes = {loadedBytes.at(0), 0x0, 0x0, 0x0};

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I32Load16Signed:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 2u);

                        std::vector<uint8_t> bytes = {loadedBytes.at(0), loadedBytes.at(1), 0x0, 0x0};

                        // sign extend if the loaded byte is negative
                        if (loadedBytes.at(1) > 127) {
                            bytes[2] = bytes[3] = 0xFFu;
                        }

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I32Load16Unsigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 2u);
                        std::vector<uint8_t> bytes = {loadedBytes.at(0), loadedBytes.at(1), 0x0, 0x0};

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }
            case InstructionId::I32Load:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> bytes = thread.heap().getBytes(offset, static_cast<uint32_t>(wasm_module::Int32::instance()->size()));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }


            case InstructionId::I64Load8Signed:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 1u);

                        std::vector<uint8_t> bytes = {loadedBytes.at(0), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

                        // sign extend if the loaded byte is negative
                        if (loadedBytes.at(0) > 127) {
                            bytes[1] = bytes[2] = bytes[3] = bytes[4] = bytes[5] = bytes[6] = bytes[7] = 0xFFu;
                        }

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I64Load8Unsigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 1u);
                        std::vector<uint8_t> bytes = {loadedBytes.at(0), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I64Load16Signed:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 2u);

                        std::vector<uint8_t> bytes = {loadedBytes.at(0), loadedBytes.at(1), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

                        // sign extend if the loaded byte is negative
                        if (loadedBytes.at(1) > 127) {
                            bytes[2] = bytes[3] = bytes[4] = bytes[5] = bytes[6] = bytes[7] = 0xFFu;
                        }

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I64Load16Unsigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 2u);
                        std::vector<uint8_t> bytes = {loadedBytes.at(0), loadedBytes.at(1), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I64Load32Signed:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 4u);

                        std::vector<uint8_t> bytes =
                                {loadedBytes.at(0), loadedBytes.at(1), loadedBytes.at(2), loadedBytes.at(3), 0x0, 0x0, 0x0, 0x0};

                        // sign extend if the loaded byte is negative
                        if (loadedBytes.at(1) > 127) {
                            bytes[4] = bytes[5] = bytes[6] = bytes[7] = 0xFFu;
                        }

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I64Load32Unsigned:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> loadedBytes = thread.heap().getBytes(offset, 4u);
                        std::vector<uint8_t> bytes =
                                {loadedBytes.at(0), loadedBytes.at(1), loadedBytes.at(2), loadedBytes.at(3), 0x0, 0x0, 0x0, 0x0};

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I64Load:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> bytes = thread.heap().getBytes
                                (offset, static_cast<uint32_t>(wasm_module::Int64::instance()->size()));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::F32Load:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> bytes = thread.heap().getBytes
                                (offset, static_cast<uint32_t>(wasm_module::Float32::instance()->size()));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::F64Load:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        std::vector<uint8_t> bytes = thread.heap().getBytes
                                (offset, static_cast<uint32_t>(wasm_module::Float64::instance()->size()));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance()->localType());
                        result.setValue(bytes);
                        return result;
                }

            case InstructionId::I32Store8:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().at(0))
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        wasm_module::Variable value = state.results().at(1);
                        thread.heap().setBytes(offset, {value.data().at(0)});
                        return StepResult(value);
                }

            case InstructionId::I32Store16:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().at(0))
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        wasm_module::Variable value = state.results().at(1);
                        thread.heap().setBytes(offset, {value.data().at(0), value.data().at(1)});
                        return StepResult(value);
                }

            case InstructionId::I64Store8:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().at(0))
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        wasm_module::Variable value = state.results().at(1);
                        thread.heap().setBytes(offset, {value.data().at(0)});
                        return StepResult(value);
                }

            case InstructionId::I64Store16:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().at(0))
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        wasm_module::Variable value = state.results().at(1);
                        thread.heap().setBytes(offset, {value.data().at(0), value.data().at(1)});
                        return StepResult(value);
                }

            case InstructionId::I64Store32:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().at(0))
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        wasm_module::Variable value = state.results().at(1);
                        thread.heap().setBytes(offset, {value.data().at(0), value.data().at(1), value.data().at(2), value.data().at(3)});
                        return StepResult(value);
                }

            case InstructionId::F32Store:
            case InstructionId::F64Store:
            case InstructionId::I32Store:
            case InstructionId::I64Store:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().at(0))
                                          + static_cast<LoadStoreInstruction&>(instruction).offset();

                        wasm_module::Variable value = state.results().at(1);
                        thread.heap().setBytes(offset, value.data());
                        return StepResult(value);
                }

            /******************************************************
             **************** Float 32 Operations *****************
             ******************************************************/

            case InstructionId::F32Add:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, left + right);
                        return result;
                }

            case InstructionId::F32Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, left - right);
                        return result;
                }
            case InstructionId::F32Mul:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, left * right);
                        return result;
                }

            case InstructionId::F32Div:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, left / right);
                        return result;
                }

            case InstructionId::F32Abs:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));
                        if (value < 0)
                            value = -value;

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, value);
                        return result;
                }

            case InstructionId::F32Neg:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));
                        value = -value;

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, value);
                        return result;
                }

            case InstructionId::F32CopySign:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, std::copysign(left, right));
                        return result;
                }
            case InstructionId::F32Ceil:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, std::ceil(value));
                        return result;
                }

            case InstructionId::F32Floor:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, std::floor(value));
                        return result;
                }
            case InstructionId::F32Trunc:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, std::trunc(value));
                        return result;
                }

            case InstructionId::F32Nearest:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        float ceilValue = std::ceil(value);
                        float floorValue = std::floor(value);

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());

                        float ceilDiff = fabsf(ceilValue - value);
                        float floorDiff = fabsf(floorValue - value);

                        if (ceilDiff == floorDiff) {
                            if (value < 0)
                                value = ceilValue;
                            else
                                value = floorValue;
                        } else if (ceilDiff < floorDiff) {
                            value = ceilValue;
                        } else {
                            value = floorValue;
                        }
                        wasm_module::Float32::setValue(result, value);
                        return result;
                }

            case InstructionId::F32Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left == right ? 1 : 0);
                        return result;
                }

            case InstructionId::F32NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left != right ? 1 : 0);
                        return result;
                }
            case InstructionId::F32LesserThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left < right ? 1 : 0);
                        return result;
                }
            case InstructionId::F32LesserEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left <= right ? 1 : 0);
                        return result;
                }
            case InstructionId::F32GreaterThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left > right ? 1 : 0);
                        return result;
                }
            case InstructionId::F32GreaterEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left >= right ? 1 : 0);
                        return result;
                }
            case InstructionId::F32Sqrt:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, std::sqrt(value));
                        return result;
                }
            case InstructionId::F32Min:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, left < right ? left : right);
                        return result;
                }
            case InstructionId::F32Max:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        wasm_module::Float32::setValue(result, left > right ? left : right);
                        return result;
                }


            /******************************************************
             **************** Float 64 Operations *****************
             ******************************************************/

            case InstructionId::F64Add:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, left + right);
                        return result;
                }

            case InstructionId::F64Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, left - right);
                        return result;
                }
            case InstructionId::F64Mul:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));
                        double resultValue = left * right;

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, resultValue);
                        return result;
                }

            case InstructionId::F64Div:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, left / right);
                        return result;
                }

            case InstructionId::F64Abs:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));
                        if (value < 0)
                            value = -value;

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, value);
                        return result;
                }

            case InstructionId::F64Neg:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));
                        value = -value;

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, value);
                        return result;
                }

            case InstructionId::F64CopySign:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, std::copysign(left, right));
                        return result;
                }
            case InstructionId::F64Ceil:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, std::ceil(value));
                        return result;
                }

            case InstructionId::F64Floor:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, std::floor(value));
                        return result;
                }
            case InstructionId::F64Trunc:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, std::trunc(value));
                        return result;
                }

            case InstructionId::F64Nearest:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        double ceilValue = std::ceil(value);
                        double floorValue = std::floor(value);

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        double ceilDiff = fabs(ceilValue - value);
                        double floorDiff = fabs(floorValue - value);

                        if (ceilDiff == floorDiff) {
                            if (value < 0)
                                value = ceilValue;
                            else
                                value = floorValue;
                        } else if (ceilDiff < floorDiff) {
                            value = ceilValue;
                        } else {
                            value = floorValue;
                        }
                        wasm_module::Float64::setValue(result, value);
                        return result;
                }

            case InstructionId::F64Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left == right ? 1 : 0);
                        return result;
                }

            case InstructionId::F64NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left != right ? 1 : 0);
                        return result;
                }
            case InstructionId::F64LesserThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left < right ? 1 : 0);
                        return result;
                }
            case InstructionId::F64LesserEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left <= right ? 1 : 0);
                        return result;
                }
            case InstructionId::F64GreaterThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left > right ? 1 : 0);
                        return result;
                }
            case InstructionId::F64GreaterEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        wasm_module::Int32::setValue(result, left >= right ? 1 : 0);
                        return result;
                }
            case InstructionId::F64Sqrt:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, std::sqrt(value));
                        return result;
                }
            case InstructionId::F64Min:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, left < right ? left : right);
                        return result;
                }
            case InstructionId::F64Max:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        wasm_module::Float64::setValue(result, left > right ? left : right);
                        return result;
                }

            /******************************************************
             ************** Conversion Operations ***************
             ******************************************************/

            case InstructionId::I32Wrap:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        memcpy(result.value(), state.results().at(0).value(), result.type().size());
                        return result;
                }

            case InstructionId::I32TruncSignedF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = (int32_t) wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = value;
                        return result;
                }

            case InstructionId::I32TruncSignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = (int32_t) wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = value;
                        return result;
                }

            case InstructionId::I32TruncUnsignedF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = (uint32_t) wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = value;
                        return result;
                }

            case InstructionId::I32TruncUnsignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = (uint32_t) wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = value;
                        return result;
                }


            case InstructionId::I32ReinterpretF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int32::instance());
                        memcpy(result.value(), state.results().at(0).value(), result.type().size());
                        return result;
                }

            case InstructionId::I64ExtendSignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = wasm_module::Int32::getValue(state.results().at(0));

                        wasm_module::Variable result = (int64_t) value;
                        return result;
                }

            case InstructionId::I64ExtendUnsignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        wasm_module::Variable result = (uint64_t) value;
                        return result;
                }

            case InstructionId::I64TruncSignedF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = (int64_t) value;
                        return result;
                }

            case InstructionId::I64TruncSignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = (int64_t) value;
                        return result;
                }

            case InstructionId::I64TruncUnsignedF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = (uint64_t) value;
                        return result;
                }

            case InstructionId::I64TruncUnsignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = (uint64_t) value;
                        return result;
                }

            case InstructionId::I64ReinterpretF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Int64::instance());
                        memcpy(result.value(), state.results().at(0).value(), result.type().size());
                        return result;
                }

            case InstructionId::F32DemoteF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        wasm_module::Variable result = (float) value;
                        return result;
                }

            case InstructionId::F32ConvertSignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = wasm_module::Int32::getValue(state.results().at(0));

                        wasm_module::Variable result = (float) value;
                        return result;
                }

            case InstructionId::F32ConvertSignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int64_t value = wasm_module::Int64::getValue(state.results().at(0));

                        wasm_module::Variable result = (float) value;
                        return result;
                }

            case InstructionId::F32ConvertUnsignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        wasm_module::Variable result = (float) value;
                        return result;
                }

            case InstructionId::F32ConvertUnsignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        wasm_module::Variable result = (float) value;
                        return result;
                }

            case InstructionId::F32ReinterpretI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float32::instance());
                        memcpy(result.value(), state.results().at(0).value(), result.type().size());
                        return result;
                }

            case InstructionId::F64PromoteF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        wasm_module::Variable result = (double) value;
                        return result;
                }


            case InstructionId::F64ConvertSignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = wasm_module::Int32::getValue(state.results().at(0));

                        wasm_module::Variable result = (double) value;
                        return result;
                }

            case InstructionId::F64ConvertSignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int64_t value = wasm_module::Int64::getValue(state.results().at(0));

                        wasm_module::Variable result = (double) value;
                        return result;
                }

            case InstructionId::F64ConvertUnsignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        wasm_module::Variable result = (double) value;
                        return result;
                }

            case InstructionId::F64ConvertUnsignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        wasm_module::Variable result = (double) value;
                        return result;
                }

            case InstructionId::F64ReinterpretI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        wasm_module::Variable result = wasm_module::Variable(wasm_module::Float64::instance());
                        memcpy(result.value(), state.results().at(0).value(), result.type().size());
                        return result;
                }


            default:
                throw UnknownInstruction(std::string("Unknown Instruction " + instruction.name()));
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
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
#include "../../wasm-module/src/instructions/InstructionId.h"

namespace wasmint {

    StepResult InstructionExecutor::execute(const wasm_module::Instruction &instruction, Thread &thread) {
        using namespace wasm_module;

        InstructionState &state = thread.getInstructionState();

        using namespace wasm_module;

        InstructionId::Value id = instruction.id();

        switch (id) {

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

                        return Variable::createUInt32(resultValue);
                }
            case InstructionId::I32Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        return Variable::createInt32(left - right);
                }
            case InstructionId::I32Mul:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        return Variable::createInt32(left * right);
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

                        return Variable::createInt32(left / right);
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

                        return Variable::createUInt32(left / right);

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
                        if (right < 0)
                            right = -right;

                        return Variable::createInt32(left % right);

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

                        return Variable::createInt32(left % right);

                }
            case InstructionId::I32And:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createUInt32(left & right);
                }

            case InstructionId::I32Or:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createUInt32(left | right);
                }

            case InstructionId::I32Xor:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createUInt32(left ^ right);
                }

            case InstructionId::I32ShiftLeft:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        right %= 32;

                        return Variable::createUInt32(left << right);
                }

            case InstructionId::I32ShiftRightZeroes:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        right %= 32;

                        return Variable::createUInt32(left >> right);
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

                        right %= 32;

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

                        return Variable::createUInt32(resultInt);
                }

            case InstructionId::I32Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left == right);
                }
            case InstructionId::I32NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left != right);
                }

            case InstructionId::I32LessThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        return Variable::createBool(left < right);
                }

            case InstructionId::I32LessEqualSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        return Variable::createBool(left <= right);
                }

            case InstructionId::I32LessThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left < right);
                }

            case InstructionId::I32LessEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left <= right);
                }

            case InstructionId::I32GreaterThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        return Variable::createBool(left > right);
                }

            case InstructionId::I32GreaterEqualSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int32_t left = wasm_module::Int32::getValue(state.results().at(0));
                        int32_t right = wasm_module::Int32::getValue(state.results().at(1));

                        return Variable::createBool(left >= right);
                }

            case InstructionId::I32GreaterThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left > right);
                }

            case InstructionId::I32GreaterEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint32_t left = wasm_module::Int32::getUnsignedValue(state.results().at(0));
                        uint32_t right = wasm_module::Int32::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left >= right);
                }


            case InstructionId::I32CountLeadingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        uint32_t leadingZeroes = 0;

                        if (value == 0) {
                            return Variable::createUInt32(32);
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

                        return Variable::createUInt32(leadingZeroes);
                }
            case InstructionId::I32CountTrailingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        uint32_t trailingZeroes = 0;

                        if (value == 0) {
                            return Variable::createUInt32(32);
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

                        return Variable::createUInt32(trailingZeroes);
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

                        return Variable::createUInt32(population);
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

                        return Variable::createInt64(left + right);
                }
            case InstructionId::I64Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        return Variable::createInt64(left - right);
                }
            case InstructionId::I64Mul:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        return Variable::createInt64(left * right);
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

                        return Variable::createInt64(left / right);
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

                        return Variable::createUInt64(left / right);
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
                        if (right < 0)
                            right = -right;

                        return Variable::createInt64(left % right);
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

                        return Variable::createUInt64(left % right);
                }
            case InstructionId::I64And:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createUInt64(left & right);
                }

            case InstructionId::I64Or:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createUInt64(left | right);
                }

            case InstructionId::I64Xor:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createUInt64(left ^ right);
                }

            case InstructionId::I64ShiftLeft:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        right %= 64;

                        return Variable::createUInt64(left << right);
                }

            case InstructionId::I64ShiftRightZeroes:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        right %= 64;

                        return Variable::createUInt64(left >> right);
                }

            case InstructionId::I64ShiftRightSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        right %= 64;

                        uint64_t resultInt = left >> right;

                        if ((left & (0x1ul << 63u)) != 0) {
                            uint64_t bitMask = 0;
                            for(uint64_t i = 0; i < right; i++) {
                                bitMask >>= 1u;
                                bitMask |= (0x1ul << 63u);
                            }
                            resultInt |= bitMask;
                        }

                        return Variable::createUInt64(resultInt);
                }

            case InstructionId::I64Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left == right);
                }

            case InstructionId::I64NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left != right);
                }

            case InstructionId::I64LessThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        return Variable::createBool(left < right);
                }

            case InstructionId::I64LessEqualSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        return Variable::createBool(left <= right);
                }

            case InstructionId::I64LessThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left < right);
                }

            case InstructionId::I64LessEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left <= right);
                }

            case InstructionId::I64GreaterThanSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        return Variable::createBool(left > right);
                }

            case InstructionId::I64GreaterEqualSigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        int64_t left = wasm_module::Int64::getValue(state.results().at(0));
                        int64_t right = wasm_module::Int64::getValue(state.results().at(1));

                        return Variable::createBool(left >= right);
                }

            case InstructionId::I64GreaterThanUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left > right);
                }

            case InstructionId::I64GreaterEqualUnsigned:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        uint64_t left = wasm_module::Int64::getUnsignedValue(state.results().at(0));
                        uint64_t right = wasm_module::Int64::getUnsignedValue(state.results().at(1));

                        return Variable::createBool(left >= right);
                }


            case InstructionId::I64CountLeadingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        uint32_t leadingZeroes = 0;

                        if (value == 0) {
                            return Variable::createUInt64(64);
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

                        return Variable::createUInt64(leadingZeroes);
                }
            case InstructionId::I64CountTrailingZeroes:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        uint64_t trailingZeroes = 0;

                        if (value == 0) {
                            return Variable::createUInt64(64);
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

                        return Variable::createUInt64(trailingZeroes);
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

                        return Variable::createUInt64(population);
                }

            /******************************************************
             ************** Control Flow Operations ***************
             ******************************************************/

            case InstructionId::Block:
                if (state.hasBranchValue())
                    return state.branchValue();

                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else {
                    if (state.results().empty())
                        return StepResult();
                    return StepResult(state.results().back());
                }

            case InstructionId::Branch:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    default:
                        return StepResult::createBranch(state.results().back(), dynamic_cast<const Branch&>(instruction).branchLabel());
                }

            case InstructionId::BranchIf:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    case 1:
                        return instruction.children().at(1);
                    default:
                        return StepResult::createBranch(state.results().back(), dynamic_cast<const BranchIf&>(instruction).branchLabel());
                }

            case InstructionId::Loop:
                if (state.hasBranchValue())
                    return state.branchValue();

                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else {
                    if (state.results().empty())
                        return StepResult();
                    return StepResult(state.results().back());
                }

            case InstructionId::Label:
                if (state.hasBranchValue())
                    return state.branchValue();

                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    default:
                        return state.results().back();
                }

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
                        return StepResult::createSignal(Signal::Return, state.results().front());
                }

            case InstructionId::Case:
                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else {
                    if (state.results().empty())
                        return StepResult();
                    return StepResult(state.results().back());
                }

            case InstructionId::TableSwitch:

                if (state.hasBranchValue())
                    return state.branchValue();

                if (state.state() == 0) {
                    return StepResult(instruction.children().front());
                }
                else if (state.state() == 1) {
                    std::size_t index = state.results().back().uint32();
                    state.clearResults();

                    const TableSwitch& tableSwitch = dynamic_cast<const TableSwitch&>(instruction);

                    auto target = index < tableSwitch.targets().size() ? tableSwitch.targets().at(index) : tableSwitch.defaultTarget();
                    if (target.isCase()) {
                        state.state((uint32_t) target.index() + 2);
                        return StepResult(instruction.children().at(target.index()));
                    } else if (target.isBranch()) {
                        if (target.index() == 0) {
                            return StepResult();
                        } else {
                            return StepResult::createBranch(Variable::Void(), (uint32_t) target.index() - 1);
                        }
                    } else {
                        throw std::domain_error("Only branch and case targets are supported as of now");
                    }
                } else if ((state.state() - 2) < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state() - 2));
                } else {
                    if (state.results().empty())
                        return StepResult();
                    return StepResult(state.results().back());
                }

            case InstructionId::Call:
                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else if (state.state() == instruction.children().size()) {
                    std::vector<wasm_module::Variable> parameters;
                    for (uint32_t i = 0; i < state.results().size(); i++) {
                        parameters.push_back(state.results().at(i));
                    }
                    const wasm_module::Call& functionCall = dynamic_cast<const wasm_module::Call &>(instruction);
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
                    const wasm_module::CallImport& functionCall = dynamic_cast<const wasm_module::CallImport &>(instruction);
                    return StepResult(thread.callFunction(functionCall.functionSignature.moduleName(), functionCall.functionSignature.name(), parameters));
                } else {
                    thread.leaveFunction();

                    return state.results().back();
                }
            case InstructionId::CallIndirect:
                if (state.state() < instruction.children().size()) {
                    return StepResult(instruction.children().at(state.state()));
                } else if (state.state() == instruction.children().size()) {
                    std::vector<wasm_module::Variable> parameters;
                    for (uint32_t i = 1; i < state.results().size(); i++) {
                        parameters.push_back(state.results().at(i));
                    }
                    const wasm_module::CallIndirect& functionCall = dynamic_cast<const wasm_module::CallIndirect &>(instruction);

                    const FunctionSignature& signature = functionCall.context_->indirectCallTable().getFunctionSignature(state.results().front().uint32());

                    return StepResult(thread.callFunction(signature.moduleName(), signature.name(), parameters));
                } else {
                    thread.leaveFunction();

                    return state.results().back();
                }
            case InstructionId::NativeInstruction:
                return StepResult(dynamic_cast<const wasm_module::NativeInstruction &>(instruction).call(thread.locals()));
            case InstructionId::GetLocal:
                return thread.variable(dynamic_cast<const wasm_module::GetLocal &>(instruction).localIndex);
            case InstructionId::I32Const:
            case InstructionId::I64Const:
            case InstructionId::F32Const:
            case InstructionId::F64Const:
                return StepResult(dynamic_cast<const wasm_module::Literal &>(instruction).literalValue());
            case InstructionId::HasFeature:
                {
                    Variable result(Int32::instance());
                    if (dynamic_cast<const wasm_module::HasFeature &>(instruction).featureName() == "wasm") {
                        result.uint32(1);
                    } else {
                        result.uint32(0);
                    }
                    return result;
                }
            case InstructionId::Unreachable:
                if (instruction.hasParent()) {
                    if (instruction.parent()->hasParent()) {
                        std::cerr << instruction.parent()->parent()->toSExprString() << std::endl;
                    } else {
                        std::cerr << instruction.parent()->toSExprString() << std::endl;
                    }
                }

                return StepResult(Signal::AssertTrap);
            case InstructionId::SetLocal:
                switch (state.state()) {
                    case 0:
                        return instruction.children().at(0);
                    default:
                        wasm_module::Variable result = thread.variable(
                                dynamic_cast<const wasm_module::SetLocal &>(instruction).localIndex) = state.results().at(0);
                        return result;
                }

            case InstructionId::Nop:
                return StepResult();

            case InstructionId::GrowMemory:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        // TODO risky conversion
                        thread.heap().grow((uint32_t) value);

                        return StepResult(Variable::Void());
                }

            case InstructionId::PageSize:
                return Variable::createUInt32(4096u);

            case InstructionId::MemorySize:
                return Variable::createUInt32((uint32_t) thread.heap().size());

            /******************************************************
             ************** Load / Store Operations ***************
             ******************************************************/
            case InstructionId::I32Load8Signed:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t offset = wasm_module::Int32::getUnsignedValue(state.results().back())
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

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
                                          + static_cast<const LoadStoreInstruction&>(instruction).offset();

                        wasm_module::Variable value = state.results().at(1);
                        thread.heap().setBytes(offset, value.data());
                        return StepResult(value);
                }

            /******************************************************
             ***************** Select Operations ******************
             ******************************************************/

            case InstructionId::I32Select:
            case InstructionId::I64Select:
            case InstructionId::F32Select:
            case InstructionId::F64Select:
                switch (state.state()) {
                    case 0:
                    case 1:
                    case 2:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        if (state.results().at(0).int32()) {
                            return state.results().at(1);
                        } else {
                            return state.results().at(2);
                        }
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

                        if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                            return Variable::createFloat32(std::numeric_limits<float>::quiet_NaN());
                        }

                        return Variable::createFloat32(left + right);
                }

            case InstructionId::F32Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && !std::signbit(right)) {
                            return Variable::createFloat32(std::numeric_limits<float>::quiet_NaN());
                        }

                        return Variable::createFloat32(left - right);
                }
            case InstructionId::F32Mul:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        if (std::isinf(left) && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                            return Variable::createFloat32(std::numeric_limits<float>::quiet_NaN());
                        }

                        return Variable::createFloat32(left * right);
                }

            case InstructionId::F32Div:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        if (left == 0 && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                            return Variable::createFloat32(std::numeric_limits<float>::quiet_NaN());
                        }

                        return Variable::createFloat32(left / right);
                }

            case InstructionId::F32Abs:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));
                        if (std::signbit(value))
                            value = -value;

                        return Variable::createFloat32(value);
                }

            case InstructionId::F32Neg:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));
                        value = -value;

                        return Variable::createFloat32(value);
                }

            case InstructionId::F32CopySign:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        return Variable::createFloat32(std::copysign(left, right));
                }
            case InstructionId::F32Ceil:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        return Variable::createFloat32(std::ceil(value));
                }

            case InstructionId::F32Floor:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        return Variable::createFloat32(std::floor(value));
                }
            case InstructionId::F32Trunc:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        return Variable::createFloat32(std::trunc(value));
                }

            case InstructionId::F32Nearest:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        value = nearbyintf(value);

                        return Variable::createFloat32(value);
                }

            case InstructionId::F32Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        return Variable::createBool(left == right);
                }

            case InstructionId::F32NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        return Variable::createBool(left != right);
                }
            case InstructionId::F32LesserThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        return Variable::createBool(left < right);
                }
            case InstructionId::F32LesserEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        return Variable::createBool(left <= right);
                }
            case InstructionId::F32GreaterThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        return Variable::createBool(left > right);
                }
            case InstructionId::F32GreaterEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        return Variable::createBool(left >= right);
                }
            case InstructionId::F32Sqrt:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        if (std::isnan(value)) {
                            uint32_t asInt = Utils::reinterpretFloatAsInt(value);
                            asInt |= 0x400000;
                            return Variable::createFloat32(Utils::reinterpretIntAsFloat(asInt));
                        }

                        if (value == -0.0f && std::signbit(value)) {
                            return Variable::createFloat32(-0.0f);
                        }

                        if (std::signbit(value))
                            return Variable::createFloat32(std::numeric_limits<float>::quiet_NaN());

                        return Variable::createFloat32(std::sqrt(value));
                }
            case InstructionId::F32Min:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        if (std::isnan(left)) {
                            uint32_t asInt = Utils::reinterpretFloatAsInt(left);
                            asInt |= 0x400000;
                            return Variable::createFloat32(Utils::reinterpretIntAsFloat(asInt));
                        }
                        if (std::isnan(right)) {
                            uint32_t asInt = Utils::reinterpretFloatAsInt(right);
                            asInt |= 0x400000;
                            return Variable::createFloat32(Utils::reinterpretIntAsFloat(asInt));
                        }

                        if (left == right) {
                            auto leftSign = std::signbit(left);
                            auto rightSign = std::signbit(right);
                            if (leftSign && !rightSign) {
                                return Variable::createFloat32(left);
                            } else if (!leftSign && rightSign) {
                                return Variable::createFloat32(right);
                            }
                        }
                        return Variable::createFloat32(left < right ? left : right);
                }
            case InstructionId::F32Max:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        float left = wasm_module::Float32::getValue(state.results().at(0));
                        float right = wasm_module::Float32::getValue(state.results().at(1));

                        if (std::isnan(left)) {
                            uint32_t asInt = Utils::reinterpretFloatAsInt(left);
                            asInt |= 0x400000;
                            return Variable::createFloat32(Utils::reinterpretIntAsFloat(asInt));
                        }
                        if (std::isnan(right)) {
                            uint32_t asInt = Utils::reinterpretFloatAsInt(right);
                            asInt |= 0x400000;
                            return Variable::createFloat32(Utils::reinterpretIntAsFloat(asInt));
                        }

                        if (left == right) {
                            auto leftSign = std::signbit(left);
                            auto rightSign = std::signbit(right);
                            if (leftSign && !rightSign) {
                                return Variable::createFloat32(right);
                            } else if (!leftSign && rightSign) {
                                return Variable::createFloat32(left);
                            }
                        }
                        return Variable::createFloat32(left > right ? left : right);
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

                        if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                            return Variable::createFloat64(std::numeric_limits<double>::quiet_NaN());
                        }

                        return Variable::createFloat64(left + right);
                }

            case InstructionId::F64Sub:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && !std::signbit(right)) {
                            return Variable::createFloat64(std::numeric_limits<double>::quiet_NaN());
                        }

                        return Variable::createFloat64(left - right);
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

                        if (std::isinf(left) && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                            return Variable::createFloat64(std::numeric_limits<double>::quiet_NaN());
                        }

                        return Variable::createFloat64(resultValue);
                }

            case InstructionId::F64Div:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        if (left == 0 && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                            return Variable::createFloat64(std::numeric_limits<double>::quiet_NaN());
                        }

                        return Variable::createFloat64(left / right);
                }

            case InstructionId::F64Abs:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));
                        if (std::signbit(value))
                            value = -value;

                        return Variable::createFloat64(value);
                }

            case InstructionId::F64Neg:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));
                        value = -value;

                        return Variable::createFloat64(value);
                }

            case InstructionId::F64CopySign:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        return Variable::createFloat64(std::copysign(left, right));
                }
            case InstructionId::F64Ceil:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        return Variable::createFloat64(std::ceil(value));
                }

            case InstructionId::F64Floor:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        return Variable::createFloat64(std::floor(value));
                }
            case InstructionId::F64Trunc:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        return Variable::createFloat64(std::trunc(value));
                }

            case InstructionId::F64Nearest:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        value = nearbyint(value);

                        return Variable::createFloat64(value);
                }

            case InstructionId::F64Equal:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        return Variable::createBool(left == right);
                }

            case InstructionId::F64NotEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        return Variable::createBool(left != right);
                }
            case InstructionId::F64LesserThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        return Variable::createBool(left < right);
                }
            case InstructionId::F64LesserEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        return Variable::createBool(left <= right);
                }
            case InstructionId::F64GreaterThan:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        return Variable::createBool(left > right);
                }
            case InstructionId::F64GreaterEqual:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        return Variable::createBool(left >= right);
                }
            case InstructionId::F64Sqrt:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        if (std::isnan(value)) {
                            uint64_t asInt = Utils::reinterpretDoubleAsInt(value);
                            asInt |= 0x8000000000000;
                            return Variable::createFloat64(Utils::reinterpretIntAsDouble(asInt));
                        }

                        if (value == -0.0 && std::signbit(value)) {
                            return Variable::createFloat64(-0.0);
                        }

                        if (std::signbit(value))
                            return Variable::createFloat64(std::numeric_limits<double>::quiet_NaN());

                        return Variable::createFloat64(std::sqrt(value));
                }
            case InstructionId::F64Min:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        if (std::isnan(left)) {
                            uint64_t asInt = Utils::reinterpretDoubleAsInt(left);
                            asInt |= 0x8000000000000;
                            return Variable::createFloat64(Utils::reinterpretIntAsDouble(asInt));
                        }
                        if (std::isnan(right)) {
                            uint64_t asInt = Utils::reinterpretDoubleAsInt(right);
                            asInt |= 0x8000000000000;
                            return Variable::createFloat64(Utils::reinterpretIntAsDouble(asInt));
                        }

                        if (left == right) {
                            auto leftSign = std::signbit(left);
                            auto rightSign = std::signbit(right);
                            if (leftSign && !rightSign) {
                                return Variable::createFloat64(left);
                            } else if (!leftSign && rightSign) {
                                return Variable::createFloat64(right);
                            }
                        }

                        return Variable::createFloat64(left < right ? left : right);
                }
            case InstructionId::F64Max:
                switch (state.state()) {
                    case 0:
                    case 1:
                        return StepResult(instruction.children().at(state.state()));
                    default:
                        double left = wasm_module::Float64::getValue(state.results().at(0));
                        double right = wasm_module::Float64::getValue(state.results().at(1));

                        if (std::isnan(left)) {
                            uint64_t asInt = Utils::reinterpretDoubleAsInt(left);
                            asInt |= 0x8000000000000;
                            return Variable::createFloat64(Utils::reinterpretIntAsDouble(asInt));
                        }
                        if (std::isnan(right)) {
                            uint64_t asInt = Utils::reinterpretDoubleAsInt(right);
                            asInt |= 0x8000000000000;
                            return Variable::createFloat64(Utils::reinterpretIntAsDouble(asInt));
                        }

                        if (left == right) {
                            auto leftSign = std::signbit(left);
                            auto rightSign = std::signbit(right);
                            if (leftSign && !rightSign) {
                                return Variable::createFloat64(right);
                            } else if (!leftSign && rightSign) {
                                return Variable::createFloat64(left);
                            }
                        }
                        return Variable::createFloat64(left > right ? left : right);
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

                        return Variable::createInt32(value);
                }

            case InstructionId::I32TruncSignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = (int32_t) wasm_module::Float64::getValue(state.results().at(0));

                        return Variable::createInt32(value);
                }

            case InstructionId::I32TruncUnsignedF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = (uint32_t) wasm_module::Float32::getValue(state.results().at(0));

                        return Variable::createUInt32(value);
                }

            case InstructionId::I32TruncUnsignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = (uint32_t) wasm_module::Float64::getValue(state.results().at(0));

                        return Variable::createUInt32(value);
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

                        return Variable::createInt64(value);
                }

            case InstructionId::I64ExtendUnsignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        return Variable::createUInt64(value);
                }

            case InstructionId::I64TruncSignedF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        return Variable::createInt64((int64_t) value);
                }

            case InstructionId::I64TruncSignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        return Variable::createInt64((int64_t) value);
                }

            case InstructionId::I64TruncUnsignedF32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        float value = wasm_module::Float32::getValue(state.results().at(0));

                        return Variable::createUInt64((uint64_t) value);
                }

            case InstructionId::I64TruncUnsignedF64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        double value = wasm_module::Float64::getValue(state.results().at(0));

                        return Variable::createUInt64((uint64_t) value);
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

                        return Variable::createFloat32((float) value);
                }

            case InstructionId::F32ConvertSignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = wasm_module::Int32::getValue(state.results().at(0));

                        return Variable::createFloat32((float) value);
                }

            case InstructionId::F32ConvertSignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int64_t value = wasm_module::Int64::getValue(state.results().at(0));

                        return Variable::createFloat32((float) value);
                }

            case InstructionId::F32ConvertUnsignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        return Variable::createFloat32((float) value);
                }

            case InstructionId::F32ConvertUnsignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        return Variable::createFloat32((float) value);
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

                        return Variable::createFloat64((double) value);
                }


            case InstructionId::F64ConvertSignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int32_t value = wasm_module::Int32::getValue(state.results().at(0));

                        return Variable::createFloat64((double) value);
                }

            case InstructionId::F64ConvertSignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        int64_t value = wasm_module::Int64::getValue(state.results().at(0));

                        return Variable::createFloat64((double) value);
                }

            case InstructionId::F64ConvertUnsignedI32:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint32_t value = wasm_module::Int32::getUnsignedValue(state.results().at(0));

                        return Variable::createFloat64((double) value);
                }

            case InstructionId::F64ConvertUnsignedI64:
                switch (state.state()) {
                    case 0:
                        return StepResult(instruction.children().at(0));
                    default:
                        uint64_t value = wasm_module::Int64::getUnsignedValue(state.results().at(0));

                        return Variable::createFloat64((double) value);
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

    bool InstructionExecutor::handleSignal(const wasm_module::Instruction &instruction, InstructionState &currentState, StepResult& stepResult) {

        Signal signal = stepResult.signal();
        if (signal == Signal::Branch) {
            if (stepResult.tryReduceBranchLabel(instruction.labelCount())) {
                return false;
            } else {
                if (instruction.id() == InstructionId::Block) {
                    currentState.branchValue(stepResult.result());
                } else if (instruction.id() == InstructionId::Label) {
                    currentState.branchValue(stepResult.result());
                }  else if (instruction.id() == InstructionId::TableSwitch) {
                    currentState.branchValue(stepResult.result());
                } else if (instruction.id() == InstructionId::Loop) {
                    switch(stepResult.branchLabel()) {
                        case 0:
                            currentState.state(0);
                            break;
                        case 1:
                            currentState.branchValue(stepResult.result());
                            break;
                        default:
                            throw std::domain_error("Loop has only 2 labels, but labelCount() was "
                                                    + std::to_string(instruction.labelCount()));
                    }
                } else {
                    throw std::domain_error("Instruction " + instruction.dataString() + " is no valid branch target. "
                                                                                                "This is an internal interpreter error");
                }
                return true;
            }
        }

        return false;
    }

}
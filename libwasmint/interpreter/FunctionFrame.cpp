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


#include "FunctionFrame.h"
#include <cmath>
#include <limits>
#include <iostream>
#include <instructions/Instructions.h>
#include "VMThread.h"
#include <Module.h>
#include "WasmintVM.h"

namespace wasmint {

void FunctionFrame::stepInternal(VMThread &runner, Heap &heap) {

    uint32_t opcode;
    popFromCode<uint32_t>(&opcode);

    //dumpStatus((ByteOpcodes::Values) opcode, opcodeData);

    switch (opcode) {
        /******************************************************
         ***************** Int 32 Operations ******************
         ******************************************************/
        case ByteOpcodes::I32Add: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left + right);
            break;
        }
        case ByteOpcodes::I32Sub: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left - right);
            break;
        }
        case ByteOpcodes::I32Mul: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left * right);
            break;
        }
        case ByteOpcodes::I32DivSigned: {
            int32_t right = pop<int32_t>();
            int32_t left = pop<int32_t>();
            if (left == std::numeric_limits<int32_t>::min() && right == -1)
                return runner.trap("integer overflow");

            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left / right);
            break;
        }
        case ByteOpcodes::I32DivUnsigned: {
            uint32_t right = pop<uint32_t>();
            uint32_t left = pop<uint32_t>();
            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left / right);
            break;
        }
        case ByteOpcodes::I32RemainderSigned: {
            int32_t right = pop<int32_t>();
            int32_t left = pop<int32_t>();
            if (right < 0)
                right = -right;
            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left % right);
            break;
        }
        case ByteOpcodes::I32RemainderUnsigned: {
            uint32_t right = pop<uint32_t>();
            uint32_t left = pop<uint32_t>();
            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left % right);
            break;
        }
        case ByteOpcodes::I32And: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left & right);
            break;
        }

        case ByteOpcodes::I32Or: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left | right);
            break;
        }

        case ByteOpcodes::I32Xor: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left ^ right);
            break;
        }

        case ByteOpcodes::I32ShiftLeft: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left << right);
            break;
        }

        case ByteOpcodes::I32ShiftRightZeroes: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left >> (right & 32));
            break;
        }

        case ByteOpcodes::I32ShiftRightSigned: {
            uint32_t right = pop<uint32_t>();
            uint32_t left = pop<uint32_t>();

            right %= 32;

            uint32_t resultInt = left >> right;

            if (((int32_t) left) < 0) {
                if ((left & (0x1u << 31u)) != 0) {
                    uint32_t bitMask = 0;
                    for (uint32_t i = 0; i < right; i++) {
                        bitMask >>= 1u;
                        bitMask |= (0x1u << 31u);
                    }
                    resultInt |= bitMask;
                }
            }

            push(resultInt);
            break;
        }

        case ByteOpcodes::I32EqualZero: {
            auto op = pop<uint32_t>();
            push(op == 0);
            break;
        }
        case ByteOpcodes::I32Equal: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left == right);
            break;
        }
        case ByteOpcodes::I32NotEqual: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left != right);
            break;
        }

        case ByteOpcodes::I32LessThanSigned: {
            auto right = pop<int32_t>();
            auto left = pop<int32_t>();
            push(left < right);
            break;
        }

        case ByteOpcodes::I32LessEqualSigned: {
            auto right = pop<int32_t>();
            auto left = pop<int32_t>();
            push(left <= right);
            break;
        }

        case ByteOpcodes::I32LessThanUnsigned: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left < right);
            break;
        }

        case ByteOpcodes::I32LessEqualUnsigned: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left <= right);
            break;
        }

        case ByteOpcodes::I32GreaterThanSigned: {
            auto right = pop<int32_t>();
            auto left = pop<int32_t>();
            push(left > right);
            break;
        }

        case ByteOpcodes::I32GreaterEqualSigned: {
            auto right = pop<int32_t>();
            auto left = pop<int32_t>();
            push(left >= right);
            break;
        }

        case ByteOpcodes::I32GreaterThanUnsigned: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left > right);
            break;
        }

        case ByteOpcodes::I32GreaterEqualUnsigned: {
            auto right = pop<uint32_t>();
            auto left = pop<uint32_t>();
            push(left >= right);
            break;
        }

        case ByteOpcodes::I32CountLeadingZeroes: {
            uint32_t value = pop<uint32_t>();

            uint32_t leadingZeroes = 0;

            if (value == 0) {
                push(32);
            } else {
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
                push(leadingZeroes);
            }
            break;
        }
        case ByteOpcodes::I32CountTrailingZeroes: {
            uint32_t value = pop<uint32_t>();

            uint32_t trailingZeroes = 0;

            if (value == 0) {
                push(32);
            } else {
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

                push(trailingZeroes);
            }
            break;
        }

        case ByteOpcodes::I32PopulationCount: {
            uint32_t value = pop<uint32_t>();

            uint32_t population = 0;

            while (true) {
                if ((value & 0x1u) == 1) {
                    population++;
                }
                value >>= 1;
                if (value == 0)
                    break;
            }
            push(population);
            break;
        }


            /******************************************************
             ***************** Int 64 Operations ******************
             ******************************************************/
        case ByteOpcodes::I64Add: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left + right);
            break;
        }
        case ByteOpcodes::I64Sub: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left - right);
            break;
        }
        case ByteOpcodes::I64Mul: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left * right);
            break;
        }
        case ByteOpcodes::I64DivSigned: {
            int64_t right = pop<int64_t>();
            int64_t left = pop<int64_t>();
            if (left == std::numeric_limits<int64_t>::min() && right == -1)
                return runner.trap("integer overflow");

            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left / right);
            break;
        }
        case ByteOpcodes::I64DivUnsigned: {
            uint64_t right = pop<uint64_t>();
            uint64_t left = pop<uint64_t>();
            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left / right);
            break;
        }
        case ByteOpcodes::I64RemainderSigned: {
            int64_t right = pop<int64_t>();
            int64_t left = pop<int64_t>();
            if (right < 0)
                right = -right;
            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left % right);
            break;
        }
        case ByteOpcodes::I64RemainderUnsigned: {
            uint64_t right = pop<uint64_t>();
            uint64_t left = pop<uint64_t>();
            if (right == 0)
                return runner.trap("integer divide by zero");
            push(left % right);
            break;
        }
        case ByteOpcodes::I64And: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left & right);
            break;
        }

        case ByteOpcodes::I64Or: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left | right);
            break;
        }

        case ByteOpcodes::I64Xor: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left ^ right);
            break;
        }

        case ByteOpcodes::I64ShiftLeft: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left << (right % 64));
            break;
        }

        case ByteOpcodes::I64ShiftRightZeroes: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left >> (right % 64));
            break;
        }

        case ByteOpcodes::I64ShiftRightSigned: {
            uint64_t right = pop<uint64_t>();
            uint64_t left = pop<uint64_t>();

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

            push(resultInt);
            break;
        }

        case ByteOpcodes::I64EqualZero: {
            auto op = pop<uint64_t>();
            push(op == 0);
            break;
        }
        case ByteOpcodes::I64Equal: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left == right);
            break;
        }
        case ByteOpcodes::I64NotEqual: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left != right);
            break;
        }

        case ByteOpcodes::I64LessThanSigned: {
            auto right = pop<int64_t>();
            auto left = pop<int64_t>();
            push(left < right);
            break;
        }

        case ByteOpcodes::I64LessEqualSigned: {
            auto right = pop<int64_t>();
            auto left = pop<int64_t>();
            push(left <= right);
            break;
        }

        case ByteOpcodes::I64LessThanUnsigned: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left < right);
            break;
        }

        case ByteOpcodes::I64LessEqualUnsigned: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left <= right);
            break;
        }

        case ByteOpcodes::I64GreaterThanSigned: {
            auto right = pop<int64_t>();
            auto left = pop<int64_t>();
            push(left > right);
            break;
        }

        case ByteOpcodes::I64GreaterEqualSigned: {
            auto right = pop<int64_t>();
            auto left = pop<int64_t>();
            push(left >= right);
            break;
        }

        case ByteOpcodes::I64GreaterThanUnsigned: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left > right);
            break;
        }

        case ByteOpcodes::I64GreaterEqualUnsigned: {
            auto right = pop<uint64_t>();
            auto left = pop<uint64_t>();
            push(left >= right);
            break;
        }

        case ByteOpcodes::I64CountLeadingZeroes: {
            uint64_t value = pop<uint64_t>();

            uint64_t leadingZeroes = 0;

            if (value == 0) {
                push(64);
            } else {
                while (true) {
                    if ((value & (0x1ul << 63u)) == 0) {
                        leadingZeroes++;
                        value <<= 1;
                    } else {
                        break;
                    }
                    if (value == 0)
                        break;
                }
                push(leadingZeroes);
            }
            break;
        }
        case ByteOpcodes::I64CountTrailingZeroes: {
            uint64_t value = pop<uint64_t>();

            uint64_t trailingZeroes = 0;

            if (value == 0) {
                push(64);
            } else {
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

                push(trailingZeroes);
            }
            break;
        }

        case ByteOpcodes::I64PopulationCount: {
            uint64_t value = pop<uint64_t>();

            uint64_t population = 0;

            while (true) {
                if ((value & 0x1u) == 1) {
                    population++;
                }
                value >>= 1;
                if (value == 0)
                    break;
            }
            push(population);
            break;
        }

            /******************************************************
             ************** Control Flow Operations ***************
             ******************************************************/

        case ByteOpcodes::Branch:
            instructionPointer_ = popFromCode<uint32_t>();
            break;

        case ByteOpcodes::BranchIf:
        {
            uint32_t jumpOffset = popFromCode<uint32_t>();
            if (pop<uint32_t>()) {
                instructionPointer_ = jumpOffset;
            }
            break;
        }
        case ByteOpcodes::BranchIfNot:
        {
            uint32_t jumpOffset = popFromCode<uint32_t>();
            if (!pop<uint32_t>()) {
                instructionPointer_ = jumpOffset;
            }
            break;
        }

        case ByteOpcodes::Return:
        {
            runner.finishFrame(pop<uint64_t>());
            break;
        }

        case ByteOpcodes::TableSwitch:
        {
            uint32_t tableSize = popFromCode<uint32_t>();
            uint32_t tableIndex = pop<uint32_t>();
            if (tableIndex < tableSize) {
                // multiply tableIndex with 4 as each address in the jump table is 4 bytes long
                instructionPointer_ = peekFromCode<uint32_t>(tableIndex * 4u);
            } else {
                // multiply tableSize with 4 as each address in the jump table is 4 bytes long
                // tableSize because the address behind the table is the default jump target
                instructionPointer_ = peekFromCode<uint32_t>(tableSize * 4u);
            }
            break;
        }

        case ByteOpcodes::CallImport:
        case ByteOpcodes::Call:
        {
            uint32_t functionId = popFromCode<uint32_t>();
            uint32_t parameterSize = popFromCode<uint32_t>();
            runner.enterFunction(functionId, parameterSize);
            break;
        }
        case ByteOpcodes::CallIndirect:
        {
            int16_t signedLocalFunctionId = pop<int16_t>();
            if (signedLocalFunctionId < 0) {
                runner.trap("undefined table index " + std::to_string(signedLocalFunctionId));
                break;
            }
            uint16_t localFunctionId = (uint16_t) signedLocalFunctionId;
            uint16_t neededIndex = popFromCode<uint16_t>();
            try {
                const wasm_module::FunctionSignature& signature = function_->function().module().context().indirectCallTable().getFunctionSignature(localFunctionId);
                std::size_t index = signature.index();
                if (index == neededIndex) {
                    uint32_t functionId = (uint32_t) runner.machine().getIndex(signature.moduleName(), signature.name());
                    uint16_t parameterSize = popFromCode<uint16_t>();
                    runner.enterFunction(functionId, parameterSize);
                } else {
                    runner.trap("indirect call signature mismatch");
                }
            } catch (const wasm_module::UnknownLocalFunctionId& ex) {
                runner.trap("undefined table index " + std::to_string(localFunctionId));
            }
            break;
        }
        case ByteOpcodes::SetLocal:
            setVariable(popFromCode<uint16_t>(), pop<uint64_t>());
            popFromCode<uint16_t>(); // pop alignment data
            break;
        case ByteOpcodes::TeeLocal:
            setVariable(popFromCode<uint16_t>(), peek<uint64_t>());
            popFromCode<uint16_t>(); // pop alignment data
            break;
        case ByteOpcodes::GetLocal:
            push(getVariable(popFromCode<uint16_t>()));
            popFromCode<uint16_t>(); // pop alignment data
            break;

        case ByteOpcodes::ClearStackPreserveTop: {
            uint64_t top = stack_.pop<uint64_t>();
            stack_.clear();
            stack_.push(top);
            break;
        }
        case ByteOpcodes::Drop:
            stack_.pop<uint64_t>();
            break;

        case ByteOpcodes::I32Const:
            push(popFromCode<uint32_t>());
            break;
        case ByteOpcodes::I64Const:
            push(popFromCode<uint64_t>());
            break;
        case ByteOpcodes::F32Const:
            push(popFromCode<float>());
            break;
        case ByteOpcodes::F64Const:
            push(popFromCode<double>());
            break;
        case ByteOpcodes::Unreachable:
            runner.trap("unreachable executed");
            break;

        case ByteOpcodes::Nop:
            break;

        case ByteOpcodes::GrowMemory: {
            uint32_t value = pop<uint32_t>();

            size_t oldSize = heap.pageCount();
            // TODO risky conversion
            if (heap.growPages((uint32_t) value)) {
                push(oldSize);
            } else {
                push((uint32_t) -1);
            }
            break;
        }

        case ByteOpcodes::PageSize:
            push((uint32_t) heap.pageSize());
            break;

        case ByteOpcodes::CurrentMemory:
            push((uint32_t) (heap.size() / heap.pageSize()));
            break;


            /******************************************************
             ************** Load / Store Operations ***************
             ******************************************************/
        case ByteOpcodes::I32Load8Signed:
        {
            int8_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I32Load8Unsigned:
        {
            uint8_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I32Load16Signed:
        {
            int16_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I32Load16Unsigned:
        {
            uint16_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                runner.trap("out of bounds memory access");
            push(value);
            break;
        }
        case ByteOpcodes::I32Load:
        {
            uint32_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I64Load8Signed:
        {
            int8_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I64Load8Unsigned: {
            uint8_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I64Load16Signed: {
            int16_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I64Load16Unsigned: {
            uint16_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I64Load32Signed: {
            int32_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I64Load32Unsigned: {
            uint32_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::I64Load: {
            uint64_t value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::F32Load: {
            float value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }

        case ByteOpcodes::F64Load: {
            double value;
            if (!heap.getStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(), &value))
                return runner.trap("out of bounds memory access");
            push(value);
            break;
        }
        case ByteOpcodes::I32Store8: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<uint8_t>()))
                return runner.trap("out of bounds memory access");
            break;
        }

        case ByteOpcodes::I32Store16: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<uint16_t>()))
                return runner.trap("out of bounds memory access");
            break;
        }

        case ByteOpcodes::I64Store8: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<uint8_t>()))
                return runner.trap("out of bounds memory access");
            break;
        }

        case ByteOpcodes::I64Store16: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<uint16_t>()))
                return runner.trap("out of bounds memory access");
            break;
        }

        case ByteOpcodes::I64Store32: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<uint32_t>()))
                return runner.trap("out of bounds memory access");
            break;
        }

        case ByteOpcodes::F32Store: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<float>()))
                return runner.trap("out of bounds memory access");
            break;
        }
        case ByteOpcodes::F64Store: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<double>()))
                return runner.trap("out of bounds memory access");
            break;
        }
        case ByteOpcodes::I32Store: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<uint32_t>()))
                return runner.trap("out of bounds memory access");
            break;
        }
        case ByteOpcodes::I64Store: {
            if (!heap.setStaticOffset(pop<uint32_t>(), popFromCode<uint32_t>(),
                                      pop<uint64_t>()))
                return runner.trap("out of bounds memory access");
            break;
        }

            /******************************************************
             ***************** Select Operations ******************
             ******************************************************/

        case ByteOpcodes::I32Select:
        case ByteOpcodes::F32Select:
        case ByteOpcodes::I64Select:
        case ByteOpcodes::F64Select: {
            auto condition = pop<uint32_t>();
            auto falseResult = pop<uint64_t>();
            auto trueResult = pop<uint64_t>();
            if (condition) {
                pop<uint64_t>();
                push(pop<uint64_t>());
            } else {
                push(pop<uint64_t>());
            }
            break;
        }
            /******************************************************
             **************** Float 32 Operations *****************
             ******************************************************/

        case ByteOpcodes::F32Add: {
            float right = pop<float>();
            float left = pop<float>();
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                push(std::numeric_limits<float>::quiet_NaN());
            } else {
                push(left + right);
            }

            break;
        }

        case ByteOpcodes::F32Sub: {
            float right = pop<float>();
            float left = pop<float>();
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && !std::signbit(right)) {
                push(std::numeric_limits<float>::quiet_NaN());
            } else {
                push(left - right);
            }

            break;
        }
        case ByteOpcodes::F32Mul: {
            float right = pop<float>();
            float left = pop<float>();
            if (std::isinf(left) && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                push(std::numeric_limits<float>::quiet_NaN());
            } else {
                push(left * right);
            }

            break;
        }

        case ByteOpcodes::F32Div: {
            float right = pop<float>();
            float left = pop<float>();
            if (left == 0 && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                push(std::numeric_limits<float>::quiet_NaN());
            } else {
                push(left / right);
            }

            break;
        }

        case ByteOpcodes::F32Abs: {
            float value = pop<float>();

            if (std::signbit(value)) {
                value = -value;
            }
            push(value);

            break;
        }

        case ByteOpcodes::F32Neg:
            push(-pop<float>());
            break;


        case ByteOpcodes::F32CopySign: {
            float right = pop<float>();
            float left = pop<float>();
            push(std::copysign(left, right));
            break;
        }

        case ByteOpcodes::F32Ceil:
            push(std::ceil(pop<float>()));
            break;

        case ByteOpcodes::F32Floor:
            push(std::floor(pop<float>()));
            break;
        case ByteOpcodes::F32Trunc:
            push(std::trunc(pop<float>()));
            break;

        case ByteOpcodes::F32Nearest:
            push(nearbyintf(pop<float>()));
            break;

        case ByteOpcodes::F32Equal: {
            auto right = pop<float>();
            auto left = pop<float>();
            push(left == right);
            break;
        }

        case ByteOpcodes::F32NotEqual: {
            auto right = pop<float>();
            auto left = pop<float>();
            push(left != right);
            break;
        }

        case ByteOpcodes::F32LesserThan: {
            auto right = pop<float>();
            auto left = pop<float>();
            push(left < right);
            break;
        }

        case ByteOpcodes::F32LesserEqual: {
            auto right = pop<float>();
            auto left = pop<float>();
            push(left <= right);
            break;
        }

        case ByteOpcodes::F32GreaterThan: {
            auto right = pop<float>();
            auto left = pop<float>();
            push(left > right);
            break;
        }

        case ByteOpcodes::F32GreaterEqual: {
            auto right = pop<float>();
            auto left = pop<float>();
            push(left >= right);
            break;
        }

        case ByteOpcodes::F32Sqrt: {
            float value = peek<float>();
            if (std::isnan(value)) {
                uint32_t asInt = pop<uint32_t>();
                asInt |= 0x400000;
                push(asInt);
                break;
            }
            pop<float>();
            if (value == -0.0f && std::signbit(value)) {
                push(-0.0f);
            } else if (std::signbit(value)) {
                push(std::numeric_limits<float>::quiet_NaN());
            } else {
                push(std::sqrt(value));
            }

            break;
        }
        case ByteOpcodes::F32Min: {
            float right = peek<float>();

            if (std::isnan(right)) {
                uint32_t asInt = pop<uint32_t>();
                asInt |= 0x400000;

                pop<float>(); // pop left arg
                push(asInt);

                break;
            }
            // pop right arg
            pop<float>();
            float left = peek<float>();

            if (std::isnan(left)) {
                uint32_t asInt = pop<uint32_t>();
                asInt |= 0x400000;
                push(asInt);
                break;
            }
            // pop left arg
            pop<float>();

            if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    push(left);
                } else if (!leftSign && rightSign) {
                    push(right);
                } else {
                    push(left < right ? left : right);
                }
            } else {
                push(left < right ? left : right);
            }
            break;
        }
        case ByteOpcodes::F32Max: {
            float right = peek<float>();

            if (std::isnan(right)) {
                uint32_t asInt = pop<uint32_t>();
                asInt |= 0x400000;

                pop<float>(); // pop left arg
                push(asInt);

                break;
            }
            // pop right arg
            pop<float>();
            float left = peek<float>();

            if (std::isnan(left)) {
                uint32_t asInt = pop<uint32_t>();
                asInt |= 0x400000;
                push(asInt);
                break;
            }
            // pop left arg
            pop<float>();

            if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    push(left);
                } else if (!leftSign && rightSign) {
                    push(right);
                } else {
                    push(left > right ? left : right);
                }
            } else {
                push(left > right ? left : right);
            }
            break;
        }


            /******************************************************
             **************** Float 64 Operations *****************
             ******************************************************/

        case ByteOpcodes::F64Add: {
            double right = pop<double>();
            double left = pop<double>();
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                push(std::numeric_limits<double>::quiet_NaN());
            } else {
                push(left + right);
            }

            break;
        }

        case ByteOpcodes::F64Sub: {
            double right = pop<double>();
            double left = pop<double>();
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && !std::signbit(right)) {
                push(std::numeric_limits<double>::quiet_NaN());
            } else {
                push(left - right);
            }

            break;
        }
        case ByteOpcodes::F64Mul: {
            double right = pop<double>();
            double left = pop<double>();
            if (std::isinf(left) && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                push(std::numeric_limits<double>::quiet_NaN());
            } else {
                push(left * right);
            }

            break;
        }

        case ByteOpcodes::F64Div: {
            double right = pop<double>();
            double left = pop<double>();
            if (left == 0 && right == 0 && !std::signbit(left) && !std::signbit(right)) {
                push(std::numeric_limits<double>::quiet_NaN());
            } else {
                push(left / right);
            }

            break;
        }

        case ByteOpcodes::F64Abs: {
            double value = pop<double>();

            if (std::signbit(value)) {
                value = -value;
            }
            push(value);

            break;
        }

        case ByteOpcodes::F64Neg:
            push(-pop<double>());
            break;


        case ByteOpcodes::F64CopySign: {
            double right = pop<double>();
            double left = pop<double>();
            push(std::copysign(left, right));
            break;
        }

        case ByteOpcodes::F64Ceil:
            push(std::ceil(pop<double>()));
            break;

        case ByteOpcodes::F64Floor:
            push(std::floor(pop<double>()));
            break;
        case ByteOpcodes::F64Trunc:
            push(std::trunc(pop<double>()));
            break;

        case ByteOpcodes::F64Nearest:
            push(nearbyint(pop<double>()));
            break;

        case ByteOpcodes::F64Equal: {
            auto right = pop<double>();
            auto left = pop<double>();
            push(left == right);
            break;
        }

        case ByteOpcodes::F64NotEqual: {
            auto right = pop<double>();
            auto left = pop<double>();
            push(left != right);
            break;
        }

        case ByteOpcodes::F64LesserThan: {
            auto right = pop<double>();
            auto left = pop<double>();
            push(left < right);
            break;
        }

        case ByteOpcodes::F64LesserEqual: {
            auto right = pop<double>();
            auto left = pop<double>();
            push(left <= right);
            break;
        }

        case ByteOpcodes::F64GreaterThan: {
            auto right = pop<double>();
            auto left = pop<double>();
            push(left > right);
            break;
        }

        case ByteOpcodes::F64GreaterEqual: {
            auto right = pop<double>();
            auto left = pop<double>();
            push(left >= right);
            break;
        }

        case ByteOpcodes::F64Sqrt: {
            double value = peek<double>();
            if (std::isnan(value)) {
                uint64_t asInt = pop<uint64_t>();
                asInt |= 0x8000000000000;
                push(asInt);
                break;
            }
            pop<double>();
            if (value == -0.0 && std::signbit(value)) {
                push(-0.0);
            } else if (std::signbit(value)) {
                push(std::numeric_limits<double>::quiet_NaN());
            } else {
                push(std::sqrt(value));
            }

            break;
        }

        case ByteOpcodes::F64Min: {
            double right = peek<double>();

            if (std::isnan(right)) {
                uint64_t asInt = pop<uint64_t>();
                asInt |= 0x8000000000000;

                pop<double>(); // pop left arg
                push(asInt);
                break;
            }
            pop<double>(); // pop right arg

            double left = peek<double>();

            if (std::isnan(left)) {
                uint64_t asInt = pop<uint64_t>();
                asInt |= 0x8000000000000;

                push(asInt);
                break;
            }

            pop<double>(); // pop left arg

            if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    push(left);
                } else if (!leftSign && rightSign) {
                    push(right);
                } else {
                    push(left < right ? left : right);
                }
            } else {
                push(left < right ? left : right);
            }
            break;
        }
        case ByteOpcodes::F64Max: {
            double right = peek<double>();

            if (std::isnan(right)) {
                uint64_t asInt = pop<uint64_t>();
                asInt |= 0x8000000000000;

                pop<double>(); // pop left arg
                push(asInt);
                break;
            }
            pop<double>(); // pop right arg

            double left = peek<double>();

            if (std::isnan(left)) {
                uint64_t asInt = pop<uint64_t>();
                asInt |= 0x8000000000000;

                push(asInt);
                break;
            }

            pop<double>(); // pop left arg

            if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    push(left);
                } else if (!leftSign && rightSign) {
                    push(right);
                } else {
                    push(left > right ? left : right);
                }
            } else {
                push(left > right ? left : right);
            }
            break;
        }
            /******************************************************
             ************** Conversion Operations ***************
             ******************************************************/

        case ByteOpcodes::I32Wrap: {
            push<int64_t>(pop<int32_t>());
            break;
        }
        case ByteOpcodes::I32TruncSignedF32: {
            float value = pop<float>();

            if (value >= 2.14748365e+09f)
                return runner.trap("integer overflow");
            if (value < std::numeric_limits<int32_t>::min())
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");


            int32_t result = (int32_t) value;

            push(result);
            break;
        }
        case ByteOpcodes::I32TruncSignedF64: {
            double value = pop<double>();

            if (value > 2147483647.0)
                return runner.trap("integer overflow");
            if (value < -2147483648.0)
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");

            int32_t result = (int32_t) value;

            push(result);
            break;
        }

        case ByteOpcodes::I32TruncUnsignedF32: {
            float value = pop<float>();

            if (value >= 4.2949673e+09f)
                return runner.trap("integer overflow");
            if (value <= -1.0f)
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");


            uint32_t result = (uint32_t) value;

            push(result);
            break;
        }

        case ByteOpcodes::I32TruncUnsignedF64: {
            double value = pop<double>();

            if (value >= 4294967296)
                return runner.trap("integer overflow");
            if (value <= -1.0)
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");

            uint32_t result = (uint32_t) value;

            push(result);
            break;
        }

        case ByteOpcodes::I64ExtendSignedI32:
            push<int64_t>(pop<int32_t>());
            break;

        case ByteOpcodes::I64ExtendUnsignedI32:
            push<uint64_t>(pop<uint32_t>());
            break;

        case ByteOpcodes::I64TruncSignedF32: {
            float value = pop<float>();

            if (value >= 9.22337204e+18f)
                return runner.trap("integer overflow");
            if (value < -9223372036854775808.0f)
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");

            int64_t result = (int64_t) value;

            push(result);
            break;
        }

        case ByteOpcodes::I64TruncSignedF64: {
            double value = pop<double>();

            if (value >= 9.2233720368547758e+18)
                return runner.trap("integer overflow");
            if (value < -9223372036854775808.0)
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");

            int64_t result = (int64_t) value;

            push(result);
            break;
        }

        case ByteOpcodes::I64TruncUnsignedF32: {
            float value = pop<float>();

            if (value >= 1.84467441e+19f)
                return runner.trap("integer overflow");
            if (value <= -1.0f)
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");

            uint64_t result = (uint64_t) value;

            push(result);
            break;
        }

        case ByteOpcodes::I64TruncUnsignedF64: {
            double value = pop<double>();

            if (value >= 1.8446744073709552e+19)
                return runner.trap("integer overflow");
            if (value <= -1.0)
                return runner.trap("integer overflow");
            if (std::isinf(value))
                return runner.trap("integer overflow");
            if (std::isnan(value))
                return runner.trap("invalid conversion to integer");

            uint64_t result = (uint64_t) value;

            push(result);
            break;
        }

        case ByteOpcodes::F32DemoteF64:
            push((float) pop<double>());
            break;

        case ByteOpcodes::F32ConvertSignedI32:
            push((float) pop<int32_t>());
            break;

        case ByteOpcodes::F32ConvertSignedI64:
            push((float) pop<int64_t>());
            break;

        case ByteOpcodes::F32ConvertUnsignedI32:
            push((float) pop<uint32_t>());
            break;

        case ByteOpcodes::F32ConvertUnsignedI64:
            push((float) pop<uint64_t>());
            break;

        case ByteOpcodes::F64PromoteF32:
            push((double) pop<float>());
            break;

        case ByteOpcodes::F64ConvertSignedI32:
            push((double) pop<int32_t>());
            break;

        case ByteOpcodes::F64ConvertSignedI64:
            push((double) pop<int64_t>());
            break;

        case ByteOpcodes::F64ConvertUnsignedI32:
            push((double) pop<uint32_t>());
            break;

        case ByteOpcodes::F64ConvertUnsignedI64:
            push((double) pop<uint64_t>());
            break;

        case ByteOpcodes::End:
            if (stack_.empty())
                runner.finishFrame(0);
            else
                runner.finishFrame(pop<uint64_t>());
            break;
        default:
            return runner.trap("Unknown instruction with opcode " + std::to_string(opcode));
    }
}

    void FunctionFrame::dumpStatus(ByteOpcodes::Values opcode, uint16_t opcodeData) {
        std::cout << "#########################\n";
        std::cout << "Opcode: " << ByteOpcodes::name(opcode) << " r" << opcodeData << "\n";
        std::cout << "Registers:\n";
        //for (std::size_t i = 0; i < registers_.size(); i++) {
        //    std::cout << "  r" << std::to_string(i) << " = " << registers_[i] << "| float: " << getRegister<float>(i) << "| double: " << getRegister<double>(i) << "\n";
        //}
        std::cout << "Variables:\n";
        for (std::size_t i = 0; i < variables_.size(); i++) {
            std::cout << "  " << function_->function().variableName((uint32_t) i) << " = " << variables_[i] << "\n";
        }
    }

    void FunctionFrame::step(VMThread &runner, Heap &heap) {
        stepInternal(runner, heap);
        /* Comment out for debugging *
        const wasm_module::Instruction* instruction = function_->jitCompiler().getInstruction(instructionPointer_);
        if (instruction) {
            std::cout << instruction->toSExprString() << std::endl;
        } /**/

    }

    bool FunctionFrame::stepDebug(VMThread &runner, Heap &heap) {
        stepInternal(runner, heap);
        return function_->triggerBreakpoints(runner.machine().state(), instructionPointer_);
    }
}
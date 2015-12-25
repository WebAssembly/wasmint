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
#include "ByteCodeRunner.h"

namespace wasmint {

void FunctionFrame::step(ByteCodeRunner& runner, Heap& heap) {

    const ByteOpcodes::Values opcode = function_->code().get<ByteOpcodes::Values>(instructionPointer_);
    instructionPointer_ += sizeof(uint8_t);

    const uint16_t opcodeData = function_->code().get<uint16_t>(instructionPointer_);
    instructionPointer_ += sizeof(uint16_t);


    switch (opcode) {
        /******************************************************
         ***************** Int 32 Operations ******************
         ******************************************************/
        case ByteOpcodes::I32Add:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) + getRegister<uint32_t>(opcodeData + 1));
            break;
        case ByteOpcodes::I32Sub:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) - getRegister<uint32_t>(opcodeData + 1));
            break;
        case ByteOpcodes::I32Mul:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) * getRegister<uint32_t>(opcodeData + 1));
            break;
        case ByteOpcodes::I32DivSigned: {
            int32_t left = getRegister<int32_t>(opcodeData);
            int32_t right = getRegister<int32_t>(opcodeData + 1);
            if (left == std::numeric_limits<int32_t>::min() && right == -1)
                return runner.trap("Cant execute i32.div_s INT32_MIN / -1 (overflow)");

            if (right == 0)
                return runner.trap("Cant execute i32.div_s x / 0 (division through zero)");
            setRegister<int32_t>(opcodeData, left / right);
            break;
        }
        case ByteOpcodes::I32DivUnsigned: {
            uint32_t left = getRegister<uint32_t>(opcodeData);
            uint32_t right = getRegister<uint32_t>(opcodeData + 1);
            if (right == 0)
                return runner.trap("Cant execute i32.div_u x / 0 (division through zero)");
            setRegister<uint32_t>(opcodeData, left / right);
            break;
        }
        case ByteOpcodes::I32RemainderSigned: {
            int32_t left = getRegister<int32_t>(opcodeData);
            int32_t right = getRegister<int32_t>(opcodeData + 1);
            if (right < 0)
                right = -right;
            if (right == 0)
                return runner.trap("Cant execute i32.rem_s x / 0 (division through zero)");
            setRegister<int32_t>(opcodeData, left % right);
            break;
        }
        case ByteOpcodes::I32RemainderUnsigned: {
            uint32_t left = getRegister<uint32_t>(opcodeData);
            uint32_t right = getRegister<uint32_t>(opcodeData + 1);
            if (right == 0)
                return runner.trap("Cant execute i32.rem_u x / 0 (division through zero)");
            setRegister<uint32_t>(opcodeData, left % right);
            break;
        }
        case ByteOpcodes::I32And:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) & getRegister<uint32_t>(opcodeData + 1));
            break;

        case ByteOpcodes::I32Or:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) | getRegister<uint32_t>(opcodeData + 1));
            break;

        case ByteOpcodes::I32Xor:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) ^ getRegister<uint32_t>(opcodeData + 1));
            break;

        case ByteOpcodes::I32ShiftLeft:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) << (getRegister<uint32_t>(opcodeData + 1) % 32));
            break;

        case ByteOpcodes::I32ShiftRightZeroes:
            setRegister<uint32_t>(opcodeData,
                                  getRegister<uint32_t>(opcodeData) >> (getRegister<uint32_t>(opcodeData + 1) % 32));
            break;

        case ByteOpcodes::I32ShiftRightSigned: {
            uint32_t left = getRegister<uint32_t>(opcodeData);
            uint32_t right = getRegister<uint32_t>(opcodeData + 1);

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

            setRegister<uint32_t>(opcodeData, resultInt);
            break;
        }

        case ByteOpcodes::I32Equal:
            setRegister<int32_t>(opcodeData,
                                 getRegister<uint32_t>(opcodeData) == (getRegister<uint32_t>(opcodeData + 1)));
            break;
        case ByteOpcodes::I32NotEqual:
            setRegister<int32_t>(opcodeData,
                                 getRegister<uint32_t>(opcodeData) != (getRegister<uint32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32LessThanSigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<int32_t>(opcodeData) < (getRegister<int32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32LessEqualSigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<int32_t>(opcodeData) <= (getRegister<int32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32LessThanUnsigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<uint32_t>(opcodeData) < (getRegister<uint32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32LessEqualUnsigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<uint32_t>(opcodeData) <= (getRegister<uint32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32GreaterThanSigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<int32_t>(opcodeData) > (getRegister<int32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32GreaterEqualSigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<int32_t>(opcodeData) >= (getRegister<int32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32GreaterThanUnsigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<uint32_t>(opcodeData) > (getRegister<uint32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32GreaterEqualUnsigned:
            setRegister<int32_t>(opcodeData,
                                 getRegister<uint32_t>(opcodeData) >= (getRegister<uint32_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I32CountLeadingZeroes: {
            uint32_t value = getRegister<uint32_t>(opcodeData);

            uint32_t leadingZeroes = 0;

            if (value == 0) {
                setRegister<uint32_t>(opcodeData, 32);
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
                setRegister<uint32_t>(opcodeData, leadingZeroes);
            }
            break;
        }
        case ByteOpcodes::I32CountTrailingZeroes: {
            uint32_t value = getRegister<uint32_t>(opcodeData);

            uint32_t trailingZeroes = 0;

            if (value == 0) {
                setRegister<uint32_t>(opcodeData, 32);
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

                setRegister<uint32_t>(opcodeData, trailingZeroes);
            }
            break;
        }

        case ByteOpcodes::I32PopulationCount: {
            uint32_t value = getRegister<uint32_t>(opcodeData);

            uint32_t population = 0;

            while (true) {
                if ((value & 0x1u) == 1) {
                    population++;
                }
                value >>= 1;
                if (value == 0)
                    break;
            }
            setRegister<uint32_t>(opcodeData, population);
        }


            /******************************************************
             ***************** Int 64 Operations ******************
             ******************************************************/
        case ByteOpcodes::I64Add:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) + getRegister<uint64_t>(opcodeData + 1));
            break;
        case ByteOpcodes::I64Sub:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) - getRegister<uint64_t>(opcodeData + 1));
            break;
        case ByteOpcodes::I64Mul:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) * getRegister<uint64_t>(opcodeData + 1));
            break;
        case ByteOpcodes::I64DivSigned: {
            int64_t left = getRegister<int64_t>(opcodeData);
            int64_t right = getRegister<int64_t>(opcodeData + 1);
            if (left == std::numeric_limits<int64_t>::min() && right == -1)
                return runner.trap("Cant execute i64.div_s INT64_MIN / -1 (overflow)");

            if (right == 0)
                return runner.trap("Cant execute i64.div_s x / 0 (division through zero)");
            setRegister<int64_t>(opcodeData, left / right);
            break;
        }
        case ByteOpcodes::I64DivUnsigned: {
            uint64_t left = getRegister<uint64_t>(opcodeData);
            uint64_t right = getRegister<uint64_t>(opcodeData + 1);
            if (right == 0)
                return runner.trap("Cant execute i64.div_u x / 0 (division through zero)");
            setRegister<uint64_t>(opcodeData, left / right);
            break;
        }
        case ByteOpcodes::I64RemainderSigned: {
            int64_t left = getRegister<int64_t>(opcodeData);
            int64_t right = getRegister<int64_t>(opcodeData + 1);
            if (right < 0)
                right = -right;
            if (right == 0)
                return runner.trap("Cant execute i64.rem_s x / 0 (division through zero)");
            setRegister<int64_t>(opcodeData, left % right);
            break;
        }
        case ByteOpcodes::I64RemainderUnsigned: {
            uint64_t left = getRegister<uint64_t>(opcodeData);
            uint64_t right = getRegister<uint64_t>(opcodeData + 1);
            if (right == 0)
                return runner.trap("Cant execute i64.rem_u x / 0 (division through zero)");
            setRegister<uint64_t>(opcodeData, left % right);
            break;
        }
        case ByteOpcodes::I64And:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) & getRegister<uint64_t>(opcodeData + 1));
            break;

        case ByteOpcodes::I64Or:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) | getRegister<uint64_t>(opcodeData + 1));
            break;

        case ByteOpcodes::I64Xor:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) ^ getRegister<uint64_t>(opcodeData + 1));
            break;

        case ByteOpcodes::I64ShiftLeft:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) << (getRegister<uint64_t>(opcodeData + 1) % 64));
            break;

        case ByteOpcodes::I64ShiftRightZeroes:
            setRegister<uint64_t>(opcodeData,
                                  getRegister<uint64_t>(opcodeData) >> (getRegister<uint64_t>(opcodeData + 1) % 64));
            break;

        case ByteOpcodes::I64ShiftRightSigned: {
            uint64_t left = getRegister<uint64_t>(opcodeData);
            uint64_t right = getRegister<uint64_t>(opcodeData + 1);

            right %= 64;

            uint64_t resultInt = left >> right;

            if (((int64_t) left) < 0) {
                if ((left & (0x1ul << 63ul)) != 0) {
                    uint64_t bitMask = 0;
                    for (uint64_t i = 0; i < right; i++) {
                        bitMask >>= 1u;
                        bitMask |= (0x1u << 31u);
                    }
                    resultInt |= bitMask;
                }
            }

            setRegister<uint64_t>(opcodeData, resultInt);
            break;
        }

        case ByteOpcodes::I64Equal:
            setRegister<int64_t>(opcodeData,
                                 getRegister<uint64_t>(opcodeData) == (getRegister<uint64_t>(opcodeData + 1)));
            break;
        case ByteOpcodes::I64NotEqual:
            setRegister<int64_t>(opcodeData,
                                 getRegister<uint64_t>(opcodeData) != (getRegister<uint64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64LessThanSigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<int64_t>(opcodeData) < (getRegister<int64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64LessEqualSigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<int64_t>(opcodeData) <= (getRegister<int64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64LessThanUnsigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<uint64_t>(opcodeData) < (getRegister<uint64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64LessEqualUnsigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<uint64_t>(opcodeData) <= (getRegister<uint64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64GreaterThanSigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<int64_t>(opcodeData) > (getRegister<int64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64GreaterEqualSigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<int64_t>(opcodeData) >= (getRegister<int64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64GreaterThanUnsigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<uint64_t>(opcodeData) > (getRegister<uint64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64GreaterEqualUnsigned:
            setRegister<int64_t>(opcodeData,
                                 getRegister<uint64_t>(opcodeData) >= (getRegister<uint64_t>(opcodeData + 1)));
            break;

        case ByteOpcodes::I64CountLeadingZeroes: {
            uint64_t value = getRegister<uint64_t>(opcodeData);

            uint64_t leadingZeroes = 0;

            if (value == 0) {
                setRegister<uint64_t>(opcodeData, 64);
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
                setRegister<uint64_t>(opcodeData, leadingZeroes);
            }
            break;
        }
        case ByteOpcodes::I64CountTrailingZeroes: {
            uint64_t value = getRegister<uint64_t>(opcodeData);

            uint64_t trailingZeroes = 0;

            if (value == 0) {
                setRegister<uint64_t>(opcodeData, 64);
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

                setRegister<uint64_t>(opcodeData, trailingZeroes);
            }
            break;
        }

        case ByteOpcodes::I64PopulationCount: {
            uint64_t value = getRegister<uint64_t>(opcodeData);

            uint64_t population = 0;

            while (true) {
                if ((value & 0x1u) == 1) {
                    population++;
                }
                value >>= 1;
                if (value == 0)
                    break;
            }
            setRegister<uint64_t>(opcodeData, population);
            break;
        }

            /******************************************************
             ************** Control Flow Operations ***************
             ******************************************************/

        case ByteOpcodes::Branch:
            instructionPointer_ -= sizeof(uint16_t);
            instructionPointer_ = popFromCode<uint32_t>();
            break;

        case ByteOpcodes::BranchIf:
        {
            uint32_t jumpOffset = popFromCode<uint32_t>();
            if (getRegister<uint32_t>(opcodeData)) {
                instructionPointer_ = jumpOffset;
            }
            break;
        }

        case ByteOpcodes::Return:
        {
            runner.finishFrame(getRegister<uint64_t>(opcodeData));
            break;
        }

        case ByteOpcodes::TableSwitch:
        {
            uint32_t tableSize = popFromCode<uint32_t>();
            uint32_t tableIndex = getRegister<uint32_t>(opcodeData);
            uint32_t jumpTarget;
            if (tableIndex < tableSize) {
                jumpTarget = peekFromCode<uint32_t>(tableIndex);
            } else {
                jumpTarget = peekFromCode<uint32_t>(tableSize);
            }
            instructionPointer_ = jumpTarget;
        }

        case ByteOpcodes::Call:
        {
            functionTargetRegister_ = opcodeData;
            runner.enterFunction(popFromCode<uint16_t>());

            break;
        }

        case ByteOpcodes::CallImport:
            break;
        case ByteOpcodes::CallIndirect:
            break;
        case ByteOpcodes::SetLocal:
            setVariable(popFromCode<uint16_t>(), getRegister<uint64_t>(opcodeData));
            break;
        case ByteOpcodes::GetLocal:
            setRegister(opcodeData, getVariable(popFromCode<uint16_t>()));
            break;
        case ByteOpcodes::I32Const:
            setRegister(opcodeData, popFromCode<uint32_t>());
            break;
        case ByteOpcodes::I64Const:
            setRegister(opcodeData, popFromCode<uint64_t>());
            break;
        case ByteOpcodes::F32Const:
            setRegister(opcodeData, popFromCode<float>());
            break;
        case ByteOpcodes::F64Const:
            setRegister(opcodeData, popFromCode<double>());
            break;
        case ByteOpcodes::Unreachable:
            runner.trap("Reached unreachable instruction");
            break;

        case ByteOpcodes::Nop:
            break;

        case ByteOpcodes::GrowMemory: {
            uint32_t value = getRegister<uint32_t>(opcodeData);

            if (value % heap.pageSize() != 0) {
                return runner.trap("Grow memory not multitude of page size");
            }

            // TODO risky conversion
            heap.grow((uint32_t) value);
        }

        case ByteOpcodes::PageSize:
            setRegister<uint32_t>(opcodeData, (uint32_t) heap.pageSize());
            break;

        case ByteOpcodes::MemorySize:
            setRegister<uint32_t>(opcodeData, (uint32_t) heap.size());
            break;


            /******************************************************
             ************** Load / Store Operations ***************
             ******************************************************/
        case ByteOpcodes::I32Load8Signed: {
            int8_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<int32_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I32Load8Unsigned: {
            uint8_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<uint32_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I32Load16Signed: {
            int16_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<int32_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I32Load16Unsigned: {
            uint16_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<uint32_t>(opcodeData, value);
            break;
        }
        case ByteOpcodes::I32Load: {
            uint32_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<uint32_t>(opcodeData, value);
            break;
        }


        case ByteOpcodes::I64Load8Signed: {
            int8_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<int64_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I64Load8Unsigned: {
            uint8_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<uint64_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I64Load16Signed: {
            int16_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<int64_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I64Load16Unsigned: {
            uint16_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<uint64_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I64Load32Signed: {
            int32_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<int64_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I64Load32Unsigned: {
            uint32_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<uint64_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::I64Load: {
            uint64_t value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<uint64_t>(opcodeData, value);
            break;
        }

        case ByteOpcodes::F32Load: {
            float value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<float>(opcodeData, value);
            break;
        }

        case ByteOpcodes::F64Load: {
            double value;
            if (!heap.getStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(), &value))
                runner.trap("Memory trap");
            setRegister<double>(opcodeData, value);
            break;
        }
        case ByteOpcodes::I32Store8: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<uint8_t>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }

        case ByteOpcodes::I32Store16: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<uint16_t>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }

        case ByteOpcodes::I64Store8: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<uint8_t>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }

        case ByteOpcodes::I64Store16: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<uint16_t>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }

        case ByteOpcodes::I64Store32: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<uint32_t>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }

        case ByteOpcodes::F32Store: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<float>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }
        case ByteOpcodes::F64Store: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<double>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }
        case ByteOpcodes::I32Store: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<uint32_t>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }
        case ByteOpcodes::I64Store: {
            if (!heap.setStaticOffset(getRegister<uint32_t>(opcodeData), popFromCode<uint32_t>(),
                                      getRegister<uint64_t>(opcodeData + 1)))
                runner.trap("Memory trap");
            break;
        }

            /******************************************************
             ***************** Select Operations ******************
             ******************************************************/

        case ByteOpcodes::I32Select:
        case ByteOpcodes::F32Select: {
            if (getRegister<uint32_t>(opcodeData)) {
                setRegister<uint32_t>(opcodeData, getRegister<uint32_t>(opcodeData + 1));
            } else {
                setRegister<uint32_t>(opcodeData, getRegister<uint32_t>(opcodeData + 2));
            }
            break;
        }
        case ByteOpcodes::I64Select:
        case ByteOpcodes::F64Select: {
            if (getRegister<uint32_t>(opcodeData)) {
                setRegister<uint64_t>(opcodeData, getRegister<uint64_t>(opcodeData + 1));
            } else {
                setRegister<uint64_t>(opcodeData, getRegister<uint64_t>(opcodeData + 2));
            }
            break;
        }
            /******************************************************
             **************** Float 32 Operations *****************
             ******************************************************/

        case ByteOpcodes::F32Add: {
            float left = getRegister<float>(opcodeData);
            float right = getRegister<float>(opcodeData + 1);
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                setRegister<float>(opcodeData, std::numeric_limits<float>::quiet_NaN());
            } else {
                setRegister<float>(opcodeData, getRegister<float>(opcodeData) + getRegister<float>(opcodeData + 1));
            }

            break;
        }

        case ByteOpcodes::F32Sub: {
            float left = getRegister<float>(opcodeData);
            float right = getRegister<float>(opcodeData + 1);
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                setRegister<float>(opcodeData, std::numeric_limits<float>::quiet_NaN());
            } else {
                setRegister<float>(opcodeData, getRegister<float>(opcodeData) - getRegister<float>(opcodeData + 1));
            }

            break;
        }
        case ByteOpcodes::F32Mul: {
            float left = getRegister<float>(opcodeData);
            float right = getRegister<float>(opcodeData + 1);
            if (std::isinf(left) && right == 0 && !std::signbit(left) && std::signbit(right)) {
                setRegister<float>(opcodeData, std::numeric_limits<float>::quiet_NaN());
            } else {
                setRegister<float>(opcodeData, getRegister<float>(opcodeData) * getRegister<float>(opcodeData + 1));
            }

            break;
        }

        case ByteOpcodes::F32Div: {
            float left = getRegister<float>(opcodeData);
            float right = getRegister<float>(opcodeData + 1);
            if (left == 0 && right == 0 && !std::signbit(left) && std::signbit(right)) {
                setRegister<float>(opcodeData, std::numeric_limits<float>::quiet_NaN());
            } else {
                setRegister<float>(opcodeData, getRegister<float>(opcodeData) / getRegister<float>(opcodeData + 1));
            }

            break;
        }

        case ByteOpcodes::F32Abs: {
            float value = getRegister<float>(opcodeData);

            if (std::signbit(value)) {
                value = -value;
            }
            setRegister<float>(opcodeData, value);

            break;
        }

        case ByteOpcodes::F32Neg:
            setRegister<float>(opcodeData, -getRegister<float>(opcodeData));
            break;


        case ByteOpcodes::F32CopySign:
            setRegister<float>(opcodeData,
                               std::copysign(getRegister<float>(opcodeData), getRegister<float>(opcodeData + 1)));
            break;

        case ByteOpcodes::F32Ceil:
            setRegister<float>(opcodeData, std::ceil(getRegister<float>(opcodeData)));
            break;

        case ByteOpcodes::F32Floor:
            setRegister<float>(opcodeData, std::floor(getRegister<float>(opcodeData)));
            break;
        case ByteOpcodes::F32Trunc:
            setRegister<float>(opcodeData, std::trunc(getRegister<float>(opcodeData)));
            break;

        case ByteOpcodes::F32Nearest:
            setRegister<float>(opcodeData, nearbyintf(getRegister<float>(opcodeData)));
            break;

        case ByteOpcodes::F32Equal:
            setRegister<float>(opcodeData, getRegister<float>(opcodeData) == getRegister<float>(opcodeData + 1));
            break;

        case ByteOpcodes::F32NotEqual:
            setRegister<float>(opcodeData, getRegister<float>(opcodeData) != getRegister<float>(opcodeData + 1));
            break;

        case ByteOpcodes::F32LesserThan:
            setRegister<float>(opcodeData, getRegister<float>(opcodeData) < getRegister<float>(opcodeData + 1));
            break;

        case ByteOpcodes::F32LesserEqual:
            setRegister<float>(opcodeData, getRegister<float>(opcodeData) <= getRegister<float>(opcodeData + 1));
            break;

        case ByteOpcodes::F32GreaterThan:
            setRegister<float>(opcodeData, getRegister<float>(opcodeData) > getRegister<float>(opcodeData + 1));
            break;

        case ByteOpcodes::F32GreaterEqual:
            setRegister<float>(opcodeData, getRegister<float>(opcodeData) >= getRegister<float>(opcodeData + 1));
            break;
        case ByteOpcodes::F32Sqrt: {
            float value = getRegister<float>(opcodeData);
            if (std::isnan(value)) {
                uint32_t asInt = getRegister<uint32_t>(opcodeData);
                asInt |= 0x400000;
                setRegister<uint32_t>(opcodeData, asInt);
            }
            else if (value == -0.0f && std::signbit(value)) {
                setRegister<float>(opcodeData, -0.0f);
            } else if (std::signbit(value)) {
                setRegister<float>(opcodeData, std::numeric_limits<float>::quiet_NaN());
            } else {
                setRegister<float>(opcodeData, std::sqrt(value));
            }

            break;
        }
        case ByteOpcodes::F32Min: {
            float left = getRegister<float>(opcodeData);
            float right = getRegister<float>(opcodeData + 1);

            if (std::isnan(left)) {
                uint32_t asInt = getRegister<uint32_t>(opcodeData);
                asInt |= 0x400000;
                setRegister<uint32_t>(opcodeData, asInt);
            } else if (std::isnan(right)) {
                uint32_t asInt = getRegister<uint32_t>(opcodeData + 1);
                asInt |= 0x400000;
                setRegister<uint32_t>(opcodeData, asInt);
            } else if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    setRegister<float>(opcodeData, left);
                } else if (!leftSign && rightSign) {
                    setRegister<float>(opcodeData, right);
                } else {
                    setRegister<float>(opcodeData, left < right ? left : right);
                }
            } else {
                setRegister<float>(opcodeData, left < right ? left : right);
            }
            break;
        }
        case ByteOpcodes::F32Max: {
            float left = getRegister<float>(opcodeData);
            float right = getRegister<float>(opcodeData + 1);

            if (std::isnan(left)) {
                uint32_t asInt = getRegister<uint32_t>(opcodeData);
                asInt |= 0x400000;
                setRegister<uint32_t>(opcodeData, asInt);
            } else if (std::isnan(right)) {
                uint32_t asInt = getRegister<uint32_t>(opcodeData + 1);
                asInt |= 0x400000;
                setRegister<uint32_t>(opcodeData, asInt);
            } else if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    setRegister<float>(opcodeData, right);
                } else if (!leftSign && rightSign) {
                    setRegister<float>(opcodeData, left);
                } else {
                    setRegister<float>(opcodeData, left > right ? left : right);
                }
            } else {
                setRegister<float>(opcodeData, left > right ? left : right);
            }
            break;
        }


            /******************************************************
             **************** Float 64 Operations *****************
             ******************************************************/

        case ByteOpcodes::F64Add: {
            double left = getRegister<double>(opcodeData);
            double right = getRegister<double>(opcodeData + 1);
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                setRegister<double>(opcodeData, std::numeric_limits<double>::quiet_NaN());
            } else {
                setRegister<double>(opcodeData, getRegister<double>(opcodeData) + getRegister<double>(opcodeData + 1));
            }

            break;
        }

        case ByteOpcodes::F64Sub: {
            double left = getRegister<double>(opcodeData);
            double right = getRegister<double>(opcodeData + 1);
            if (std::isinf(left) && std::isinf(right) && !std::signbit(left) && std::signbit(right)) {
                setRegister<double>(opcodeData, std::numeric_limits<double>::quiet_NaN());
            } else {
                setRegister<double>(opcodeData, getRegister<double>(opcodeData) - getRegister<double>(opcodeData + 1));
            }

            break;
        }
        case ByteOpcodes::F64Mul: {
            double left = getRegister<double>(opcodeData);
            double right = getRegister<double>(opcodeData + 1);
            if (std::isinf(left) && right == 0 && !std::signbit(left) && std::signbit(right)) {
                setRegister<double>(opcodeData, std::numeric_limits<double>::quiet_NaN());
            } else {
                setRegister<double>(opcodeData, getRegister<double>(opcodeData) * getRegister<double>(opcodeData + 1));
            }

            break;
        }

        case ByteOpcodes::F64Div: {
            double left = getRegister<double>(opcodeData);
            double right = getRegister<double>(opcodeData + 1);
            if (left == 0 && right == 0 && !std::signbit(left) && std::signbit(right)) {
                setRegister<double>(opcodeData, std::numeric_limits<double>::quiet_NaN());
            } else {
                setRegister<double>(opcodeData, getRegister<double>(opcodeData) / getRegister<double>(opcodeData + 1));
            }

            break;
        }

        case ByteOpcodes::F64Abs: {
            double value = getRegister<double>(opcodeData);

            if (std::signbit(value)) {
                value = -value;
            }
            setRegister<double>(opcodeData, value);

            break;
        }

        case ByteOpcodes::F64Neg:
            setRegister<double>(opcodeData, -getRegister<double>(opcodeData));
            break;


        case ByteOpcodes::F64CopySign:
            setRegister<double>(opcodeData,
                                std::copysign(getRegister<double>(opcodeData), getRegister<double>(opcodeData + 1)));
            break;

        case ByteOpcodes::F64Ceil:
            setRegister<double>(opcodeData, std::ceil(getRegister<double>(opcodeData)));
            break;

        case ByteOpcodes::F64Floor:
            setRegister<double>(opcodeData, std::floor(getRegister<double>(opcodeData)));
            break;
        case ByteOpcodes::F64Trunc:
            setRegister<double>(opcodeData, std::trunc(getRegister<double>(opcodeData)));
            break;

        case ByteOpcodes::F64Nearest:
            setRegister<double>(opcodeData, nearbyint(getRegister<double>(opcodeData)));
            break;

        case ByteOpcodes::F64Equal:
            setRegister<double>(opcodeData, getRegister<double>(opcodeData) == getRegister<double>(opcodeData + 1));
            break;

        case ByteOpcodes::F64NotEqual:
            setRegister<double>(opcodeData, getRegister<double>(opcodeData) != getRegister<double>(opcodeData + 1));
            break;

        case ByteOpcodes::F64LesserThan:
            setRegister<double>(opcodeData, getRegister<double>(opcodeData) < getRegister<double>(opcodeData + 1));
            break;

        case ByteOpcodes::F64LesserEqual:
            setRegister<double>(opcodeData, getRegister<double>(opcodeData) <= getRegister<double>(opcodeData + 1));
            break;

        case ByteOpcodes::F64GreaterThan:
            setRegister<double>(opcodeData, getRegister<double>(opcodeData) > getRegister<double>(opcodeData + 1));
            break;

        case ByteOpcodes::F64GreaterEqual:
            setRegister<double>(opcodeData, getRegister<double>(opcodeData) >= getRegister<double>(opcodeData + 1));
            break;
        case ByteOpcodes::F64Sqrt: {
            double value = getRegister<double>(opcodeData);
            if (std::isnan(value)) {
                uint64_t asInt = getRegister<uint64_t>(opcodeData);
                asInt |= 0x400000;
                setRegister<uint64_t>(opcodeData, asInt);
            }
            else if (value == -0.0f && std::signbit(value)) {
                setRegister<double>(opcodeData, -0.0f);
            } else if (std::signbit(value)) {
                setRegister<double>(opcodeData, std::numeric_limits<double>::quiet_NaN());
            } else {
                setRegister<double>(opcodeData, std::sqrt(value));
            }

            break;
        }
        case ByteOpcodes::F64Min: {
            double left = getRegister<double>(opcodeData);
            double right = getRegister<double>(opcodeData + 1);

            if (std::isnan(left)) {
                uint64_t asInt = getRegister<uint64_t>(opcodeData);
                asInt |= 0x400000;
                setRegister<uint64_t>(opcodeData, asInt);
            } else if (std::isnan(right)) {
                uint64_t asInt = getRegister<uint64_t>(opcodeData + 1);
                asInt |= 0x400000;
                setRegister<uint64_t>(opcodeData, asInt);
            } else if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    setRegister<double>(opcodeData, left);
                } else if (!leftSign && rightSign) {
                    setRegister<double>(opcodeData, right);
                } else {
                    setRegister<double>(opcodeData, left < right ? left : right);
                }
            } else {
                setRegister<double>(opcodeData, left < right ? left : right);
            }
            break;
        }
        case ByteOpcodes::F64Max: {
            double left = getRegister<double>(opcodeData);
            double right = getRegister<double>(opcodeData + 1);

            if (std::isnan(left)) {
                uint64_t asInt = getRegister<uint64_t>(opcodeData);
                asInt |= 0x400000;
                setRegister<uint64_t>(opcodeData, asInt);
            } else if (std::isnan(right)) {
                uint64_t asInt = getRegister<uint64_t>(opcodeData + 1);
                asInt |= 0x400000;
                setRegister<uint64_t>(opcodeData, asInt);
            } else if (left == right) {
                auto leftSign = std::signbit(left);
                auto rightSign = std::signbit(right);
                if (leftSign && !rightSign) {
                    setRegister<double>(opcodeData, right);
                } else if (!leftSign && rightSign) {
                    setRegister<double>(opcodeData, left);
                } else {
                    setRegister<double>(opcodeData, left > right ? left : right);
                }
            } else {
                setRegister<double>(opcodeData, left > right ? left : right);
            }
            break;
        }
            /******************************************************
             ************** Conversion Operations ***************
             ******************************************************/

        case ByteOpcodes::I32TruncSignedF32: {
            float value = getRegister<float>(opcodeData);

            if (value >= 2.14748365e+09f)
                return runner.trap("i32.trunc_s/f32 can't handle value " + std::to_string(value));
            if (value < std::numeric_limits<int32_t>::min())
                return runner.trap("i32.trunc_s/f32 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i32.trunc_s/f32 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i32.trunc_s/f32 can't handle value " + std::to_string(value));


            int32_t result = (int32_t) value;

            setRegister<int32_t>(opcodeData, result);
            break;
        }
        case ByteOpcodes::I32TruncSignedF64: {
            double value = getRegister<double>(opcodeData);

            if (value > 2147483647.0)
                return runner.trap("i32.trunc_s/f64 can't handle value " + std::to_string(value));
            if (value < -2147483648.0)
                return runner.trap("i32.trunc_s/f64 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i32.trunc_s/f64 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i32.trunc_s/f64 can't handle value " + std::to_string(value));

            int32_t result = (int32_t) value;

            setRegister<int32_t>(opcodeData, result);
            break;
        }

        case ByteOpcodes::I32TruncUnsignedF32: {
            float value = getRegister<float>(opcodeData);

            if (value >= 4.2949673e+09f)
                return runner.trap("i32.trunc_u/f32 can't handle value " + std::to_string(value));
            if (value <= -1.0f)
                return runner.trap("i32.trunc_u/f32 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i32.trunc_u/f32 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i32.trunc_u/f32 can't handle value " + std::to_string(value));


            uint32_t result = (uint32_t) value;

            setRegister<uint32_t>(opcodeData, result);
            break;
        }

        case ByteOpcodes::I32TruncUnsignedF64: {
            double value = getRegister<double>(opcodeData);

            if (value >= 4294967296)
                return runner.trap("i32.trunc_u/f64 can't handle value " + std::to_string(value));
            if (value <= -1.0)
                return runner.trap("i32.trunc_u/f64 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i32.trunc_u/f64 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i32.trunc_u/f64 can't handle value " + std::to_string(value));

            uint32_t result = (uint32_t) value;

            setRegister<uint32_t>(opcodeData, result);
            break;
        }

        case ByteOpcodes::I64ExtendSignedI32:
            setRegister<int64_t>(opcodeData, getRegister<int32_t>(opcodeData));
            break;

        case ByteOpcodes::I64ExtendUnsignedI32:
            setRegister<uint64_t>(opcodeData, getRegister<uint32_t>(opcodeData));
            break;

        case ByteOpcodes::I64TruncSignedF32: {
            float value = getRegister<float>(opcodeData);

            if (value >= 9.22337204e+18f)
                return runner.trap("i64.trunc_s/f32 can't handle value " + std::to_string(value));
            if (value < -9223372036854775808.0f)
                return runner.trap("i64.trunc_s/f32 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i64.trunc_s/f32 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i64.trunc_s/f32 can't handle value " + std::to_string(value));

            int64_t result = (int64_t) value;

            setRegister<int64_t>(opcodeData, result);
            break;
        }

        case ByteOpcodes::I64TruncSignedF64: {
            double value = getRegister<double>(opcodeData);

            if (value >= 9.2233720368547758e+18)
                return runner.trap("i64.trunc_s/f64 can't handle value " + std::to_string(value));
            if (value < -9223372036854775808.0)
                return runner.trap("i64.trunc_s/f64 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i64.trunc_s/f64 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i64.trunc_s/f64 can't handle value " + std::to_string(value));

            int64_t result = (int64_t) value;

            setRegister<int64_t>(opcodeData, result);
            break;
        }

        case ByteOpcodes::I64TruncUnsignedF32: {
            float value = getRegister<float>(opcodeData);

            if (value >= 1.84467441e+19f)
                return runner.trap("i64.trunc_u/f32 can't handle value " + std::to_string(value));
            if (value <= -1.0f)
                return runner.trap("i64.trunc_u/f32 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i64.trunc_u/f32 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i64.trunc_u/f32 can't handle value " + std::to_string(value));

            uint64_t result = (uint64_t) value;

            setRegister<uint64_t>(opcodeData, result);
            break;
        }

        case ByteOpcodes::I64TruncUnsignedF64: {
            double value = getRegister<double>(opcodeData);

            if (value >= 1.8446744073709552e+19)
                return runner.trap("i64.trunc_u/f64 can't handle value " + std::to_string(value));
            if (value <= -1.0)
                return runner.trap("i64.trunc_u/f64 can't handle value " + std::to_string(value));
            if (std::isinf(value))
                return runner.trap("i64.trunc_u/f64 can't handle value " + std::to_string(value));
            if (std::isnan(value))
                return runner.trap("i64.trunc_u/f64 can't handle value " + std::to_string(value));

            uint64_t result = (uint64_t) value;

            setRegister<uint64_t>(opcodeData, result);
            break;
        }

        case ByteOpcodes::F32DemoteF64:
            setRegister<float>(opcodeData, (float) getRegister<double>(opcodeData));
            break;

        case ByteOpcodes::F32ConvertSignedI32:
            setRegister<float>(opcodeData, (float) getRegister<int32_t>(opcodeData));
            break;

        case ByteOpcodes::F32ConvertSignedI64:
            setRegister<float>(opcodeData, (float) getRegister<int64_t>(opcodeData));
            break;

        case ByteOpcodes::F32ConvertUnsignedI32:
            setRegister<float>(opcodeData, (float) getRegister<uint64_t>(opcodeData));
            break;

        case ByteOpcodes::F32ConvertUnsignedI64:
            setRegister<float>(opcodeData, (float) getRegister<uint64_t>(opcodeData));
            break;

        case ByteOpcodes::F64PromoteF32:
            setRegister<double>(opcodeData, (double) getRegister<float>(opcodeData));
            break;

        case ByteOpcodes::F64ConvertSignedI32:
            setRegister<double>(opcodeData, (double) getRegister<int32_t>(opcodeData));
            break;

        case ByteOpcodes::F64ConvertSignedI64:
            setRegister<double>(opcodeData, (double) getRegister<int64_t>(opcodeData));
            break;

        case ByteOpcodes::F64ConvertUnsignedI32:
            setRegister<double>(opcodeData, (double) getRegister<uint32_t>(opcodeData));
            break;

        case ByteOpcodes::F64ConvertUnsignedI64:
            setRegister<double>(opcodeData, (double) getRegister<uint64_t>(opcodeData));
            break;

        case ByteOpcodes::CopyReg:
        {
            uint16_t sourceRegister = popFromCode<uint16_t>();
            setRegister<uint64_t>(opcodeData, getRegister<uint64_t>(sourceRegister));
            break;
        }
        default:
            return runner.trap("Unknown instruction with opcode " + std::to_string(opcode));
    }
}
}
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


#include <instructions/Instructions.h>
#include "JITCompiler.h"
#include "WasmintVM.h"
#include <stdexcept>

#define Op2Case(Name) case InstructionId:: Name : \
        compileInstruction(instruction->children().at(0)); \
        compileInstruction(instruction->children().at(1)); \
        code_.appendOpcode(ByteOpcodes:: Name ); \
        code_.append(registerAllocator_(instruction)); \
        break;

#define Op1Case(Name) case InstructionId:: Name : \
        compileInstruction(instruction->children().at(0)); \
        code_.appendOpcode(ByteOpcodes:: Name ); \
        code_.append(registerAllocator_(instruction)); \
        break;

#define Op0Case(Name) case InstructionId:: Name : \
        code_.appendOpcode(ByteOpcodes:: Name ); \
        code_.append(registerAllocator_(instruction)); \
        break;

void wasmint::JITCompiler::compileInstruction(const wasm_module::Instruction* instruction) {

    instructionStartAddresses[instruction] = code_.size();
    // check that each operation is word aligned
    assert(code_.size() % 4 == 0);

    switch (instruction->id()) {
        Op2Case(I32Add)
        Op2Case(I32Sub)
        Op2Case(I32Mul)
        Op2Case(I32DivSigned)
        Op2Case(I32DivUnsigned)
        Op2Case(I32RemainderSigned)
        Op2Case(I32RemainderUnsigned)
        Op2Case(I32And)
        Op2Case(I32Or)
        Op2Case(I32Xor)
        Op2Case(I32ShiftLeft)
        Op2Case(I32ShiftRightZeroes)
        Op2Case(I32ShiftRightSigned)
        Op2Case(I32Equal)
        Op2Case(I32NotEqual)
        Op2Case(I32LessThanSigned)
        Op2Case(I32LessEqualSigned)
        Op2Case(I32LessThanUnsigned)
        Op2Case(I32LessEqualUnsigned)
        Op2Case(I32GreaterThanSigned)
        Op2Case(I32GreaterEqualSigned)
        Op2Case(I32GreaterThanUnsigned)
        Op2Case(I32GreaterEqualUnsigned)
        Op2Case(I64Add)
        Op2Case(I64Sub)
        Op2Case(I64Mul)
        Op2Case(I64DivSigned)
        Op2Case(I64DivUnsigned)
        Op2Case(I64RemainderSigned)
        Op2Case(I64RemainderUnsigned)
        Op2Case(I64And)
        Op2Case(I64Or)
        Op2Case(I64Xor)
        Op2Case(I64ShiftLeft)
        Op2Case(I64ShiftRightZeroes)
        Op2Case(I64ShiftRightSigned)
        Op2Case(I64Equal)
        Op2Case(I64NotEqual)
        Op2Case(I64LessThanSigned)
        Op2Case(I64LessEqualSigned)
        Op2Case(I64LessThanUnsigned)
        Op2Case(I64LessEqualUnsigned)
        Op2Case(I64GreaterThanSigned)
        Op2Case(I64GreaterEqualSigned)
        Op2Case(I64GreaterThanUnsigned)
        Op2Case(I64GreaterEqualUnsigned)
        Op2Case(F32Add)
        Op2Case(F32Sub)
        Op2Case(F32Mul)
        Op2Case(F32Div)
        Op2Case(F32CopySign)
        Op2Case(F32Equal)
        Op2Case(F32NotEqual)
        Op2Case(F32LesserThan)
        Op2Case(F32LesserEqual)
        Op2Case(F32GreaterThan)
        Op2Case(F32GreaterEqual)
        Op2Case(F32Min)
        Op2Case(F32Max)
        Op2Case(F64Add)
        Op2Case(F64Sub)
        Op2Case(F64Mul)
        Op2Case(F64Div)
        Op2Case(F64CopySign)
        Op2Case(F64Equal)
        Op2Case(F64NotEqual)
        Op2Case(F64LesserThan)
        Op2Case(F64LesserEqual)
        Op2Case(F64GreaterThan)
        Op2Case(F64GreaterEqual)
        Op2Case(F64Min)
        Op2Case(F64Max)

        Op1Case(I32CountLeadingZeroes)
        Op1Case(I32CountTrailingZeroes)
        Op1Case(I32PopulationCount)
        Op1Case(I64CountLeadingZeroes)
        Op1Case(I64CountTrailingZeroes)
        Op1Case(I64PopulationCount)
        Op1Case(F32Sqrt)
        Op1Case(F64Sqrt)
        Op1Case(I32TruncSignedF32)
        Op1Case(I32TruncSignedF64)
        Op1Case(I32TruncUnsignedF32)
        Op1Case(I32TruncUnsignedF64)
        Op1Case(I64ExtendSignedI32)
        Op1Case(I64ExtendUnsignedI32)
        Op1Case(I64TruncSignedF32)
        Op1Case(I64TruncSignedF64)
        Op1Case(I64TruncUnsignedF32)
        Op1Case(I64TruncUnsignedF64)
        Op1Case(F32DemoteF64)
        Op1Case(F32ConvertSignedI32)
        Op1Case(F32ConvertSignedI64)
        Op1Case(F32ConvertUnsignedI32)
        Op1Case(F32ConvertUnsignedI64)
        Op1Case(F64PromoteF32)
        Op1Case(F64ConvertSignedI32)
        Op1Case(F64ConvertSignedI64)
        Op1Case(F64ConvertUnsignedI32)
        Op1Case(F64ConvertUnsignedI64)
        Op1Case(GrowMemory)
        Op1Case(F64Abs)
        Op1Case(F64Neg)
        Op1Case(F64Ceil)
        Op1Case(F64Floor)
        Op1Case(F64Trunc)
        Op1Case(F64Nearest)
        Op1Case(F32Abs)
        Op1Case(F32Neg)
        Op1Case(F32Ceil)
        Op1Case(F32Floor)
        Op1Case(F32Trunc)
        Op1Case(F32Nearest)

        Op0Case(PageSize)
        Op0Case(MemorySize)

        case InstructionId::I32Load8Signed:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I32Load8Signed);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Load8Signed*>(instruction)->offset());
            break;
        case InstructionId::I32Load8Unsigned:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I32Load8Unsigned);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Load8Unsigned*>(instruction)->offset());
            break;
        case InstructionId::I32Load16Signed:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I32Load16Signed);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Load16Signed*>(instruction)->offset());
            break;
        case InstructionId::I32Load16Unsigned:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I32Load16Unsigned);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Load16Unsigned*>(instruction)->offset());
            break;
        case InstructionId::I32Load:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I32Load);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Load*>(instruction)->offset());
            break;
        case InstructionId::I64Load8Signed:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I64Load8Signed);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Load8Signed*>(instruction)->offset());
            break;
        case InstructionId::I64Load8Unsigned:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I64Load8Unsigned);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Load8Unsigned*>(instruction)->offset());
            break;
        case InstructionId::I64Load16Signed:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I64Load16Signed);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Load16Signed*>(instruction)->offset());
            break;
        case InstructionId::I64Load16Unsigned:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I64Load16Unsigned);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Load16Unsigned*>(instruction)->offset());
            break;
        case InstructionId::I64Load32Signed:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I64Load32Signed);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Load32Signed*>(instruction)->offset());
            break;
        case InstructionId::I64Load32Unsigned:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I64Load32Unsigned);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Load32Unsigned*>(instruction)->offset());
            break;
        case InstructionId::I64Load:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I64Load);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Load*>(instruction)->offset());
            break;
        case InstructionId::F32Load:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::F32Load);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::F32Load*>(instruction)->offset());
            break;
        case InstructionId::F64Load:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::F64Load);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::F64Load*>(instruction)->offset());
            break;

        case InstructionId::I32Store8:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::I32Store8);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Store8*>(instruction)->offset());
            break;
        case InstructionId::I32Store16:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::I32Store16);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Store16*>(instruction)->offset());
            break;
        case InstructionId::I32Store:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::I32Store);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I32Store*>(instruction)->offset());
            break;
        case InstructionId::I64Store8:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::I64Store8);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Store8*>(instruction)->offset());
            break;
        case InstructionId::I64Store16:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::I64Store16);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Store16*>(instruction)->offset());
            break;
        case InstructionId::I64Store32:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::I64Store32);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Store32*>(instruction)->offset());
            break;
        case InstructionId::I64Store:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::I64Store);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::I64Store*>(instruction)->offset());
            break;
        case InstructionId::F32Store:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::F32Store);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::F32Store*>(instruction)->offset());
            break;
        case InstructionId::F64Store:
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            code_.appendOpcode(ByteOpcodes::F64Store);
            code_.append(registerAllocator_(instruction));
            code_.append<uint32_t>(dynamic_cast<const wasm_module::F64Store*>(instruction)->offset());
            break;

        case InstructionId::GetLocal:
            code_.appendOpcode(ByteOpcodes::GetLocal);
            code_.append(registerAllocator_(instruction));
            code_.append<uint16_t>((uint16_t) dynamic_cast<const wasm_module::GetLocal*>(instruction)->localIndex);
            break;
        case InstructionId::SetLocal:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::SetLocal);
            code_.append(registerAllocator_(instruction));
            code_.append<uint16_t>((uint16_t) dynamic_cast<const wasm_module::SetLocal*>(instruction)->localIndex);
            break;
        case InstructionId::I32Const:
            code_.appendOpcode(ByteOpcodes::I32Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().uint32());
            break;
        case InstructionId::I64Const:
            code_.appendOpcode(ByteOpcodes::I64Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().uint64());
            break;
        case InstructionId::F32Const:
            code_.appendOpcode(ByteOpcodes::F32Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().float32());
            break;
        case InstructionId::F64Const:
            code_.appendOpcode(ByteOpcodes::F64Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().float64());
            break;

        case InstructionId::HasFeature:
        {
            code_.appendOpcode(ByteOpcodes::I32Const);
            code_.append(registerAllocator_(instruction));

            const std::string& feature = dynamic_cast<const wasm_module::HasFeature*>(instruction)->featureName();
            if (feature == "wasm") {
                code_.append(1);
            } else {
                code_.append(0);
            }

            break;
        }
        case InstructionId::I32Wrap:
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::I32Wrap);
            code_.append<uint16_t>(registerAllocator_(instruction));
            break;
        case InstructionId::I64ReinterpretF64:
        case InstructionId::F64ReinterpretI64:
        case InstructionId::F32ReinterpretI32:
        case InstructionId::I32ReinterpretF32:
        case InstructionId::Nop:
            for (std::size_t i = 0; i < instruction->children().size(); i++)
                compileInstruction(instruction->children()[i]);
            code_.appendOpcode(ByteOpcodes::Nop);
            code_.append<uint16_t>(0);
            break;
        case InstructionId::Unreachable:
            code_.appendOpcode(ByteOpcodes::Unreachable);
            code_.append<uint16_t>(0);
            break;

        case InstructionId::CallIndirect:
        {
            for (std::size_t i = 0; i < instruction->children().size(); i++)
                compileInstruction(instruction->children()[i]);

            const wasm_module::CallIndirect* call = dynamic_cast<const wasm_module::CallIndirect*>(instruction);

            code_.appendOpcode(ByteOpcodes::CallIndirect);
            code_.append<uint16_t>(registerAllocator_(instruction));
            code_.append<uint16_t>((uint16_t) call->functionType().index());
            code_.append<uint16_t>((uint16_t) (call->childrenTypes().size() - 1));
            // nop that will trigger when we return (just for the debugger)
            code_.appendOpcode(ByteOpcodes::Nop);
            code_.append<uint16_t>(0);
            break;
        }
        case InstructionId::CallImport:
        {
            for (std::size_t i = 0; i < instruction->children().size(); i++)
                compileInstruction(instruction->children()[i]);

            const wasm_module::CallImport* call = dynamic_cast<const wasm_module::CallImport*>(instruction);

            code_.appendOpcode(ByteOpcodes::Call);
            code_.append<uint16_t>(registerAllocator_(instruction));
            needsFunctionIndex.push_back(std::make_pair(call->functionSignature, code_.size()));
            code_.append<uint32_t>(0);
            code_.append<uint32_t>((uint32_t) instruction->children().size());
            for (std::size_t i = 0; i < instruction->children().size(); i++) {
                const wasm_module::Type* type = instruction->children().at(i)->returnType();
                if (type == wasm_module::Int32::instance()) {
                    code_.append<uint32_t>(0);
                } else if (type == wasm_module::Int64::instance()) {
                    code_.append<uint32_t>(1);
                } else if (type == wasm_module::Float32::instance()) {
                    code_.append<uint32_t>(2);
                } else if (type == wasm_module::Float64::instance()) {
                    code_.append<uint32_t>(3);
                } else if (type == wasm_module::Void::instance()) {
                    // void arguments will result in crash
                    code_.append<uint32_t>(4);
                }
            }
            // nop that will trigger when we return (just for the debugger)
            code_.appendOpcode(ByteOpcodes::Nop);
            code_.append<uint16_t>(0);
            break;
        }
        case InstructionId::Call:
        {
            for (std::size_t i = 0; i < instruction->children().size(); i++)
                compileInstruction(instruction->children()[i]);

            const wasm_module::Call* call = dynamic_cast<const wasm_module::Call*>(instruction);

            code_.appendOpcode(ByteOpcodes::Call);
            code_.append<uint16_t>(registerAllocator_(instruction));
            needsFunctionIndex.push_back(std::make_pair(call->functionSignature, code_.size()));
            code_.append<uint32_t>(0);
            code_.append<uint32_t>((uint32_t) call->childrenTypes().size());
            // nop that will trigger when we return (just for the debugger)
            code_.appendOpcode(ByteOpcodes::Nop);
            code_.append<uint16_t>(0);
            break;
        }

        case InstructionId::TableSwitch:
        {
            compileInstruction(instruction->children().at(0));
            code_.appendOpcode(ByteOpcodes::TableSwitch);
            code_.append<uint16_t>(registerAllocator_(instruction));

            const wasm_module::TableSwitch* tableSwitch = dynamic_cast<const wasm_module::TableSwitch*>(instruction);
            code_.append<uint32_t>((uint32_t) tableSwitch->targets().size());

            for (const wasm_module::TableSwitchTarget& target : tableSwitch->targets()) {
                if (target.isCase()) {
                    addBranchAddress(target.branchInformation().target(), true);
                } else if (target.isBranch()) {
                    addBranchAddress(&target.branchInformation());
                } else {
                    throw std::domain_error("Only case and branch are supported by tableswitch");
                }
            }
            if (tableSwitch->defaultTarget().isCase()) {
                addBranchAddress(tableSwitch->defaultTarget().branchInformation().target(), true);
            } else if (tableSwitch->defaultTarget().isBranch()) {
                addBranchAddress(&tableSwitch->defaultTarget().branchInformation());
            } else {
                throw std::domain_error("Only case and branch are supported by tableswitch");
            }

            for (uint32_t i = 1; i < instruction->children().size(); i++) {
                compileInstruction(tableSwitch->children().at(i));
            }

            break;
        }
        case InstructionId::Loop:
        case InstructionId::Case:
        case InstructionId::Label:
        case InstructionId::Block:
        {
            for (std::size_t i = 0; i < instruction->children().size(); i++)
                compileInstruction(instruction->children()[i]);
            code_.appendOpcode(ByteOpcodes::Nop);
            code_.append<uint16_t>(0);
            break;
        }
        case InstructionId::If:
        {
            compileInstruction(instruction->children().at(0));
            addBranchIfNot(instruction, registerAllocator_(instruction), false);
            compileInstruction(instruction->children().at(1));
            break;
        }
        case InstructionId::IfElse:
        {
            compileInstruction(instruction->children().at(0));
            addBranchIfNot(instruction->children().at(2), registerAllocator_(instruction), true);
            compileInstruction(instruction->children().at(1));
            addBranch(instruction, false);
            compileInstruction(instruction->children().at(2));
            break;
        }
        case InstructionId::Return:
        {
            compileInstruction(instruction->children().at(0));
            if (registerAllocator_.hasRegister(instruction->children().at(0)))
                addBranchCopyReg(instruction->function()->mainInstruction(),
                                 0, // target the first register which is our return register
                                 registerAllocator_(instruction->children().at(0)));
            else
                addBranch(instruction->function()->mainInstruction(), false);
            break;
        }
        case InstructionId::Branch:
        {
            compileInstruction(instruction->children().at(0));
            if (registerAllocator_.hasRegister(instruction->children().at(0)))
                addBranchCopyReg(instruction->branchInformation(),
                                 registerAllocator_(instruction->branchInformation()->target()),
                                 registerAllocator_(instruction->children().at(0)));
            else
                addBranch(instruction->branchInformation());
            break;
        }
        case InstructionId::BranchIf:
        {
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            if (registerAllocator_.hasRegister(instruction->children().at(1)))
                addBranchIfCopyReg(instruction->branchInformation(),
                                   registerAllocator_(instruction->children().at(0)),
                                   registerAllocator_(instruction->branchInformation()->target()),
                                   registerAllocator_(instruction->children().at(1)));
            else
                addBranchIf(instruction->branchInformation(), registerAllocator_(instruction->children().at(0)));
            break;
        }

        case InstructionId::I32Select:
        {
            compileInstruction(instruction->children()[0]);
            compileInstruction(instruction->children()[1]);
            compileInstruction(instruction->children()[2]);
            code_.appendOpcode(ByteOpcodes::I32Select);
            code_.append(registerAllocator_(instruction));
            break;
        }
        case InstructionId::I64Select:
        {
            compileInstruction(instruction->children()[0]);
            compileInstruction(instruction->children()[1]);
            compileInstruction(instruction->children()[2]);
            code_.appendOpcode(ByteOpcodes::I64Select);
            code_.append(registerAllocator_(instruction));
            break;
        }
        case InstructionId::F32Select:
        {
            compileInstruction(instruction->children()[0]);
            compileInstruction(instruction->children()[1]);
            compileInstruction(instruction->children()[2]);
            code_.appendOpcode(ByteOpcodes::F32Select);
            code_.append(registerAllocator_(instruction));
            break;
        }
        case InstructionId::F64Select:
        {
            compileInstruction(instruction->children()[0]);
            compileInstruction(instruction->children()[1]);
            compileInstruction(instruction->children()[2]);
            code_.appendOpcode(ByteOpcodes::F64Select);
            code_.append(registerAllocator_(instruction));
            break;
        }

        default:
            throw std::domain_error("calculateNumberOfRegisters can't handle instruction " + instruction->name());
    }

    instructionEndAddresses[instruction] = code_.size();
    instructionFinishedAddresses[code_.size()] = instruction;
}

void wasmint::JITCompiler::addBranch(const wasm_module::BranchInformation* information) {
    code_.appendOpcode(ByteOpcodes::Branch);
    addBranchAddress(information);
}

void wasmint::JITCompiler::addBranch(const wasm_module::Instruction* instruction, bool before) {
    code_.appendOpcode(ByteOpcodes::Branch);
    addBranchAddress(instruction, before);
}

void wasmint::JITCompiler::addBranchIf(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before) {
    code_.appendOpcode(ByteOpcodes::BranchIf);
    code_.append(opcodeData);
    addBranchAddress(instruction, before);
}

void wasmint::JITCompiler::addBranchIfNot(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before) {
    code_.appendOpcode(ByteOpcodes::BranchIfNot);
    code_.append(opcodeData);
    addBranchAddress(instruction, before);
}

void wasmint::JITCompiler::addBranchCopyReg(const wasm_module::BranchInformation* information, uint16_t targetReg, uint16_t srcReg) {
    if (targetReg == srcReg || information->targetsStart()) {
        addBranch(information->target(), information->targetsStart());
    } else {
        code_.appendOpcode(ByteOpcodes::BranchCopyReg);
        code_.append(targetReg);
        code_.append(srcReg);
        addBranchAddress(information->target(), false);
    }
}

void wasmint::JITCompiler::addBranchIfCopyReg(const wasm_module::BranchInformation* information, uint16_t opcodeData, uint16_t targetReg, uint16_t srcReg) {
    if (information->targetsStart()) {
        addBranchIf(information, opcodeData);
    } else {
        code_.appendOpcode(ByteOpcodes::BranchIfCopyReg);
        code_.append(opcodeData);
        code_.append(targetReg);
        code_.append(srcReg);
        addBranchAddress(information);
    }
}

void wasmint::JITCompiler::addBranchIf(const wasm_module::BranchInformation* information, uint16_t opcodeData) {
    code_.appendOpcode(ByteOpcodes::BranchIf);
    code_.append(opcodeData);
    addBranchAddress(information);
}

void wasmint::JITCompiler::addBranchIfNot(const wasm_module::BranchInformation* information, uint16_t opcodeData) {
    code_.appendOpcode(ByteOpcodes::BranchIfNot);
    code_.append(opcodeData);
    addBranchAddress(information);
}

void wasmint::JITCompiler::addBranchAddress(const wasm_module::BranchInformation* information) {
    if (information->targetsStart()) {
        needsInstructionStartAddress.push_back(std::make_pair(information->target(), code_.size()));
    } else {
        needsInstructionEndAddress.push_back(std::make_pair(information->target(), code_.size()));
    }
    code_.append<uint32_t>(0);
}

void wasmint::JITCompiler::addBranchAddress(const wasm_module::Instruction* instruction, bool before) {
    if (before) {
        needsInstructionStartAddress.push_back(std::make_pair(instruction, code_.size()));
    } else {
        needsInstructionEndAddress.push_back(std::make_pair(instruction, code_.size()));
    }
    code_.append<uint32_t>(0);
}

void wasmint::JITCompiler::addBranchCopyReg(const wasm_module::Instruction* instruction, uint16_t targetReg, uint16_t srcReg) {
    code_.appendOpcode(ByteOpcodes::BranchCopyReg);
    code_.append(targetReg);
    code_.append(srcReg);
    addBranchAddress(instruction, false);
}

void wasmint::JITCompiler::addCopyRegister(uint16_t target, uint16_t source) {
    code_.appendOpcode(ByteOpcodes::CopyReg);
    code_.append(target);
    code_.append(source);
}


void wasmint::JITCompiler::linkGlobally(WasmintVM* registerMachine) {
    for (auto pair : needsFunctionIndex) {
        auto& signature = pair.first;

        bool foundFunction = false;

        for (uint32_t i = 0; i < registerMachine->getNumberOfCompiledFunction(); i++) {
            const wasm_module::Function& function = registerMachine->getCompiledFunction(i).function();
            if (signature.moduleName() == function.moduleName() && signature.name() == function.name()) {
                code_.write<uint32_t>(pair.second, i);
                foundFunction = true;
                break;
            }
        }
        if (!foundFunction) {
            throw std::domain_error("Can't find link target " + signature.toString());
        }
    }
}

void wasmint::JITCompiler::linkLocally() {
    for (auto pair : needsInstructionStartAddress) {
        auto addressIter = instructionStartAddresses.find(pair.first);
        if (addressIter != instructionStartAddresses.end())
            code_.write(pair.second, addressIter->second);
        else
            throw std::domain_error("Can't find start address of instruction " + pair.first->toSExprString());
    }
    for (auto pair : needsInstructionEndAddress) {
        auto addressIter = instructionEndAddresses.find(pair.first);
        if (addressIter != instructionEndAddresses.end())
            code_.write(pair.second, addressIter->second);
        else
            throw std::domain_error("Can't find end address of instruction " + pair.first->toSExprString());
    }
}

void wasmint::JITCompiler::compile(const wasm_module::Function* function) {
    if (!function->isNative()) {
        registerAllocator_ = RegisterAllocator();
        registerAllocator_.allocateRegisters(function->mainInstruction());
        code_.append<uint16_t>(registerAllocator_.registersRequired());
        code_.append<uint16_t>((uint16_t) function->locals().size());
        compileInstruction(function->mainInstruction());
        code_.appendOpcode(ByteOpcodes::End);
        code_.append<uint16_t>(0);
        linkLocally();
    }
}

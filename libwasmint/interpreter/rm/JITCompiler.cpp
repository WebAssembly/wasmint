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

#define Op2Case(Name) case InstructionId:: Name : \
        compileInstruction(instruction->children()[0]); \
        compileInstruction(instruction->children()[1]); \
        code_.appendOpcode(ByteOpcodes:: Name ); \
        code_.append(registerAllocator_(instruction)); \
        break;

#define Op1Case(Name) case InstructionId:: Name : \
        compileInstruction(instruction->children()[0]); \
        code_.appendOpcode(ByteOpcodes:: Name ); \
        code_.append(registerAllocator_(instruction)); \
        break;

void wasmint::JITCompiler::compileInstruction(const wasm_module::Instruction* instruction) {

    instructionStartAddresses[instruction] = code_.size();

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
        Op2Case(F32Abs)
        Op2Case(F32Neg)
        Op2Case(F32CopySign)
        Op2Case(F32Ceil)
        Op2Case(F32Floor)
        Op2Case(F32Trunc)
        Op2Case(F32Nearest)
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
        Op2Case(F64Abs)
        Op2Case(F64Neg)
        Op2Case(F64CopySign)
        Op2Case(F64Ceil)
        Op2Case(F64Floor)
        Op2Case(F64Trunc)
        Op2Case(F64Nearest)
        Op2Case(F64Equal)
        Op2Case(F64NotEqual)
        Op2Case(F64LesserThan)
        Op2Case(F64LesserEqual)
        Op2Case(F64GreaterThan)
        Op2Case(F64GreaterEqual)
        Op2Case(F64Min)
        Op2Case(F64Max)
        Op2Case(I32Store8)
        Op2Case(I32Store16)
        Op2Case(I32Store)
        Op2Case(I64Store8)
        Op2Case(I64Store16)
        Op2Case(I64Store32)
        Op2Case(I64Store)
        Op2Case(F32Store)
        Op2Case(F64Store)

        Op1Case(I32CountLeadingZeroes)
        Op1Case(I32CountTrailingZeroes)
        Op1Case(I32PopulationCount)
        Op1Case(I64CountLeadingZeroes)
        Op1Case(I64CountTrailingZeroes)
        Op1Case(I64PopulationCount)
        Op1Case(GetLocal)
        Op1Case(SetLocal)
        Op1Case(GrowMemory)
        Op1Case(PageSize)
        Op1Case(MemorySize)
        Op1Case(I32Load8Signed)
        Op1Case(I32Load8Unsigned)
        Op1Case(I32Load16Signed)
        Op1Case(I32Load16Unsigned)
        Op1Case(I32Load)
        Op1Case(I64Load8Signed)
        Op1Case(I64Load8Unsigned)
        Op1Case(I64Load16Signed)
        Op1Case(I64Load16Unsigned)
        Op1Case(I64Load32Signed)
        Op1Case(I64Load32Unsigned)
        Op1Case(I64Load)
        Op1Case(F32Load)
        Op1Case(F64Load)
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

        case InstructionId::I32Const:
            code_.appendOpcode(ByteOpcodes::I32Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().uint32());
            break;
        case InstructionId::I64Const:
            code_.appendOpcode(ByteOpcodes::I32Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().uint64());
            break;
        case InstructionId::F32Const:
            code_.appendOpcode(ByteOpcodes::I32Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().float32());
            break;
        case InstructionId::F64Const:
            code_.appendOpcode(ByteOpcodes::I32Const);
            code_.append(registerAllocator_(instruction));
            code_.append(dynamic_cast<const wasm_module::Literal*>(instruction)->literalValue().float64());
            break;

        case InstructionId::I32Wrap:
        case InstructionId::I64ReinterpretF64:
        case InstructionId::F64ReinterpretI64:
        case InstructionId::F32ReinterpretI32:
        case InstructionId::I32ReinterpretF32:
        case InstructionId::Nop:
        case InstructionId::Unreachable:
        case InstructionId::HasFeature:
            code_.appendOpcode(ByteOpcodes::Nop);
            code_.append<uint16_t>(0);
            break;

        case InstructionId::CallIndirect:
        case InstructionId::CallImport:
        case InstructionId::Call:
        {
            for (std::size_t i = 0; i < instruction->children().size(); i++)
                compileInstruction(instruction->children()[i]);

            code_.appendOpcode(ByteOpcodes::Call);
            code_.append<uint16_t>(0);
            break;
        }

        case InstructionId::TableSwitch:
        {
            code_.appendOpcode(ByteOpcodes::TableSwitch);
            code_.append<uint16_t>(registerAllocator_(instruction));

            const wasm_module::TableSwitch* tableSwitch = dynamic_cast<const wasm_module::TableSwitch*>(instruction);
            code_.append<uint32_t>((uint32_t) tableSwitch->targets().size());

            for (const wasm_module::TableSwitchTarget& target : tableSwitch->targets()) {
                addBranch(target.branchInformation().target(), registerAllocator_(instruction), target.branchInformation().labelIndex() == 1);
            }

            break;
        }
        case InstructionId::Loop:
        case InstructionId::Case:
        case InstructionId::Block:
        {
            for (std::size_t i = 0; i < instruction->children().size(); i++)
                compileInstruction(instruction->children()[i]);
            break;
        }
        case InstructionId::BranchIf:
        {
            compileInstruction(instruction->children().at(0));
            compileInstruction(instruction->children().at(1));
            addBranchIf(instruction, registerAllocator_(instruction), false);
            break;
        }
        case InstructionId::If:
        {
            compileInstruction(instruction->children().at(0));
            addBranchIf(instruction, registerAllocator_(instruction), false);
            compileInstruction(instruction->children().at(1));
            break;
        }
        case InstructionId::IfElse:
        {
            compileInstruction(instruction->children().at(0));
            addBranchIf(instruction->children().at(2), registerAllocator_(instruction), true);
            compileInstruction(instruction->children().at(1));
            addBranch(instruction, registerAllocator_(instruction), false);
            compileInstruction(instruction->children().at(2));
            break;
        }
        case InstructionId::Return:
        {
            addBranch(instruction->function()->mainInstruction(), registerAllocator_(instruction), false);
            break;
        }
        case InstructionId::Branch:
        {
            addBranch(instruction->branchInformation(), registerAllocator_(instruction));
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
}

void wasmint::JITCompiler::addBranch(const wasm_module::BranchInformation* information, uint16_t opcodeData) {
    addBranch(information->target(), information->labelIndex() == 1, opcodeData);
}

void wasmint::JITCompiler::addBranch(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before) {
    code_.appendOpcode(ByteOpcodes::Branch);
    addBranchAddress(instruction, before);
    addCopyRegister(registerAllocator_(instruction), opcodeData);
}

void wasmint::JITCompiler::addBranchIf(const wasm_module::Instruction* instruction, uint16_t opcodeData, bool before) {
    code_.appendOpcode(ByteOpcodes::BranchIf);
    code_.append(opcodeData);
    addBranchAddress(instruction, before);
    addCopyRegister(registerAllocator_(instruction), opcodeData);
}

void wasmint::JITCompiler::addCopyRegister(uint16_t target, uint16_t source) {
    code_.appendOpcode(ByteOpcodes::BranchIf);
    code_.append(target);
    code_.append(source);
}

void wasmint::JITCompiler::addBranchAddress(const wasm_module::Instruction* instruction, bool before) {
    if (before) {
        needsInstructionStartAddress.push_back(std::make_pair(instruction, code_.size()));
    } else {
        needsInstructionEndAddress.push_back(std::make_pair(instruction, code_.size()));
    }
    code_.append<uint32_t>(0);
}
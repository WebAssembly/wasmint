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


#include <iostream>
#include "RegisterAllocator.h"


void wasmint::RegisterAllocator::allocateRegisters(const wasm_module::Instruction* instruction, std::size_t offset) {
    switch (instruction->id()) {
        case InstructionId::I32Add:
        case InstructionId::I32Sub:
        case InstructionId::I32Mul:
        case InstructionId::I32DivSigned:
        case InstructionId::I32DivUnsigned:
        case InstructionId::I32RemainderSigned:
        case InstructionId::I32RemainderUnsigned:
        case InstructionId::I32And:
        case InstructionId::I32Or:
        case InstructionId::I32Xor:
        case InstructionId::I32ShiftLeft:
        case InstructionId::I32ShiftRightZeroes:
        case InstructionId::I32ShiftRightSigned:
        case InstructionId::I32Equal:
        case InstructionId::I32NotEqual:
        case InstructionId::I32LessThanSigned:
        case InstructionId::I32LessEqualSigned:
        case InstructionId::I32LessThanUnsigned:
        case InstructionId::I32LessEqualUnsigned:
        case InstructionId::I32GreaterThanSigned:
        case InstructionId::I32GreaterEqualSigned:
        case InstructionId::I32GreaterThanUnsigned:
        case InstructionId::I32GreaterEqualUnsigned:
        case InstructionId::I64Add:
        case InstructionId::I64Sub:
        case InstructionId::I64Mul:
        case InstructionId::I64DivSigned:
        case InstructionId::I64DivUnsigned:
        case InstructionId::I64RemainderSigned:
        case InstructionId::I64RemainderUnsigned:
        case InstructionId::I64And:
        case InstructionId::I64Or:
        case InstructionId::I64Xor:
        case InstructionId::I64ShiftLeft:
        case InstructionId::I64ShiftRightZeroes:
        case InstructionId::I64ShiftRightSigned:
        case InstructionId::I64Equal:
        case InstructionId::I64NotEqual:
        case InstructionId::I64LessThanSigned:
        case InstructionId::I64LessEqualSigned:
        case InstructionId::I64LessThanUnsigned:
        case InstructionId::I64LessEqualUnsigned:
        case InstructionId::I64GreaterThanSigned:
        case InstructionId::I64GreaterEqualSigned:
        case InstructionId::I64GreaterThanUnsigned:
        case InstructionId::I64GreaterEqualUnsigned:
        case InstructionId::F32Add:
        case InstructionId::F32Sub:
        case InstructionId::F32Mul:
        case InstructionId::F32Div:
        case InstructionId::F32CopySign:
        case InstructionId::F32Equal:
        case InstructionId::F32NotEqual:
        case InstructionId::F32LesserThan:
        case InstructionId::F32LesserEqual:
        case InstructionId::F32GreaterThan:
        case InstructionId::F32GreaterEqual:
        case InstructionId::F32Min:
        case InstructionId::F32Max:
        case InstructionId::F64Add:
        case InstructionId::F64Sub:
        case InstructionId::F64Mul:
        case InstructionId::F64Div:
        case InstructionId::F64CopySign:
        case InstructionId::F64Equal:
        case InstructionId::F64NotEqual:
        case InstructionId::F64LesserThan:
        case InstructionId::F64LesserEqual:
        case InstructionId::F64GreaterThan:
        case InstructionId::F64GreaterEqual:
        case InstructionId::F64Min:
        case InstructionId::F64Max:
        case InstructionId::I32Store8:
        case InstructionId::I32Store16:
        case InstructionId::I32Store:
        case InstructionId::I64Store8:
        case InstructionId::I64Store16:
        case InstructionId::I64Store32:
        case InstructionId::I64Store:
        case InstructionId::F32Store:
        case InstructionId::F64Store:
        case InstructionId::BranchIf:
        {
            allocateRegisters(instruction->children().at(0), offset);
            allocateRegisters(instruction->children().at(1), offset + 1);
            setRegister(instruction, offset);
            break;
        }

        case InstructionId::I32CountLeadingZeroes:
        case InstructionId::I32CountTrailingZeroes:
        case InstructionId::I32PopulationCount:
        case InstructionId::I64CountLeadingZeroes:
        case InstructionId::I64CountTrailingZeroes:
        case InstructionId::I64PopulationCount:
        case InstructionId::SetLocal:
        case InstructionId::GrowMemory:
        case InstructionId::I32Load8Signed:
        case InstructionId::I32Load8Unsigned:
        case InstructionId::I32Load16Signed:
        case InstructionId::I32Load16Unsigned:
        case InstructionId::I32Load:
        case InstructionId::I64Load8Signed:
        case InstructionId::I64Load8Unsigned:
        case InstructionId::I64Load16Signed:
        case InstructionId::I64Load16Unsigned:
        case InstructionId::I64Load32Signed:
        case InstructionId::I64Load32Unsigned:
        case InstructionId::I64Load:
        case InstructionId::F32Load:
        case InstructionId::F64Load:
        case InstructionId::F32Sqrt:
        case InstructionId::F64Sqrt:
        case InstructionId::I32Wrap:
        case InstructionId::I32TruncSignedF32:
        case InstructionId::I32TruncSignedF64:
        case InstructionId::I32TruncUnsignedF32:
        case InstructionId::I32TruncUnsignedF64:
        case InstructionId::I32ReinterpretF32:
        case InstructionId::I64ExtendSignedI32:
        case InstructionId::I64ExtendUnsignedI32:
        case InstructionId::I64TruncSignedF32:
        case InstructionId::I64TruncSignedF64:
        case InstructionId::I64TruncUnsignedF32:
        case InstructionId::I64TruncUnsignedF64:
        case InstructionId::I64ReinterpretF64:
        case InstructionId::F32DemoteF64:
        case InstructionId::F32ConvertSignedI32:
        case InstructionId::F32ConvertSignedI64:
        case InstructionId::F32ConvertUnsignedI32:
        case InstructionId::F32ConvertUnsignedI64:
        case InstructionId::F32ReinterpretI32:
        case InstructionId::F64PromoteF32:
        case InstructionId::F64ConvertSignedI32:
        case InstructionId::F64ConvertSignedI64:
        case InstructionId::F64ConvertUnsignedI32:
        case InstructionId::F64ConvertUnsignedI64:
        case InstructionId::F64ReinterpretI64:
        case InstructionId::Return:
        case InstructionId::Label:
        case InstructionId::Branch:
        case InstructionId::F64Abs:
        case InstructionId::F64Neg:
        case InstructionId::F64Ceil:
        case InstructionId::F64Floor:
        case InstructionId::F64Trunc:
        case InstructionId::F64Nearest:
        case InstructionId::F32Abs:
        case InstructionId::F32Neg:
        case InstructionId::F32Ceil:
        case InstructionId::F32Floor:
        case InstructionId::F32Trunc:
        case InstructionId::F32Nearest:
        {
            allocateRegisters(instruction->children().at(0), offset);
            setRegister(instruction, offset);
            break;
        }
        case InstructionId::I32Const:
        case InstructionId::I64Const:
        case InstructionId::F32Const:
        case InstructionId::F64Const:
        case InstructionId::HasFeature:
        case InstructionId::PageSize:
        case InstructionId::CurrentMemory:
        case InstructionId::GetLocal:
        {
            setRegister(instruction, offset);
            break;
        }

        case InstructionId::Nop:
        case InstructionId::Unreachable:
        {
            break;
        }

        case InstructionId::CallIndirect:
        case InstructionId::CallImport:
        case InstructionId::Call:
        {
            for (std::size_t localOffset = 0; localOffset < instruction->children().size(); localOffset++)
                allocateRegisters(instruction->children().at(localOffset), offset + localOffset);
            setRegister(instruction, offset);
            break;
        }

        case InstructionId::TableSwitch:
        case InstructionId::Case:
        case InstructionId::Block:
        case InstructionId::Loop:
        case InstructionId::If:
        case InstructionId::IfElse:
        {
            for (const wasm_module::Instruction* child : instruction->children())
                allocateRegisters(child, offset);

            setRegister(instruction, offset);
            break;
        }

        case InstructionId::I32Select:
        case InstructionId::I64Select:
        case InstructionId::F32Select:
        case InstructionId::F64Select:
        {
            allocateRegisters(instruction->children().at(0), offset);
            allocateRegisters(instruction->children().at(1), offset + 1);
            allocateRegisters(instruction->children().at(2), offset + 2);
            setRegister(instruction, offset);
            break;
        }

        default:
            throw std::domain_error("calculateNumberOfRegisters can't handle instruction " + instruction->name());
    }



}
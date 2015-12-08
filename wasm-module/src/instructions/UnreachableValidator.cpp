#include "UnreachableValidator.h"

#include "Instruction.h"

bool wasm_module::UnreachableValidator::willExecuteUnreachable(const Instruction* instruction) {
    switch (instruction->id()) {
        case InstructionId::Unreachable:
            return true;
        case InstructionId::If:
        case InstructionId::IfElse:
        case InstructionId::I32Select:
        case InstructionId::I64Select:
        case InstructionId::F32Select:
        case InstructionId::F64Select:
            return willExecuteUnreachable(instruction->children().at(0));

        default:
        {
            for(const Instruction* child : instruction->children()) {
                if (willExecuteUnreachable(child)) {
                    return true;
                }
            }
            return false;
        }
    }
}
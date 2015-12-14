#include "UnreachableValidator.h"

#include "Instruction.h"

bool wasm_module::UnreachableValidator::willNeverEvaluate(const Instruction *instruction) {
    switch (instruction->id()) {
        case InstructionId::Unreachable:
        case InstructionId::Branch:
        case InstructionId::BranchIf:
        case InstructionId::Return:
            return true;
        case InstructionId::If:
        case InstructionId::IfElse:
            return willNeverEvaluate(instruction->children().at(0));
        default:
        {
            for(const Instruction* child : instruction->children()) {
                if (willNeverEvaluate(child)) {
                    return true;
                }
            }
            return false;
        }
    }
}
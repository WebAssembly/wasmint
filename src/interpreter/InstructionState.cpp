#include "InstructionState.h"

#include <instructions/Instruction.h>

void InstructionState::step(Thread& thread) {
    if (childInstruction != nullptr) {
        childInstruction->step(thread);
        if (childInstruction->finished()) {
            results_.push_back(childInstruction->result());
            delete childInstruction;
            childInstruction = nullptr;
        }
    } else {
        StepResult result = instruction_->execute(thread);
        if (result.newChildInstruction()) {
            childInstruction = new InstructionState(result.newChildInstruction());
        } else {
            result_ = result.result();
            finished_ = true;
        }
        state_++;
    }
}
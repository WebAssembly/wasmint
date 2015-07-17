#include "InstructionState.h"

#include <instructions/Instruction.h>

Signal InstructionState::step(Thread& thread) {
    if (childInstruction != nullptr) {
        Signal signal = childInstruction->step(thread);
        if (childInstruction->finished()) {
            results_.push_back(childInstruction->result());
            delete childInstruction;
            childInstruction = nullptr;
        }
        if (signal != Signal::None) {
            if (instruction()->handleSignal(*this, signal)) {
                delete childInstruction;
                childInstruction = nullptr;
                return Signal::None;
            }
        }
        return signal;
    } else {
        StepResult result = instruction_->execute(thread);
        if (result.newChildInstruction()) {
            childInstruction = new InstructionState(result.newChildInstruction());
        } else if (result.signal() != Signal::None) {
            return result.signal();
        } else {
            result_ = result.result();
            finished_ = true;
        }
        state_++;
    }
    return Signal::None;
}
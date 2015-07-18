#include "InstructionState.h"

#include <instructions/Instruction.h>
#include "Thread.h"
#include "InstructionExecutor.h"

Signal InstructionState::step(Thread& thread) {
    if (childInstruction != nullptr) {
        Signal signal = childInstruction->step(thread);
        if (childInstruction->finished()) {
            results_.push_back(childInstruction->result());
            delete childInstruction;
            childInstruction = nullptr;
        }
        if (signal != Signal::None) {
            if (InstructionExecutor::handleSignal(*instruction(), *this, signal)) {
                delete childInstruction;
                childInstruction = nullptr;
                return Signal::None;
            }
        }
        return signal;
    } else {
        StepResult result = InstructionExecutor::execute(*instruction(), thread);
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

InstructionState::~InstructionState() {
    if (childInstruction)
        delete childInstruction;
}
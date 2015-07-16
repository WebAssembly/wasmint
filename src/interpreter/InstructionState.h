#ifndef WASMINT_INSTRUCTIONSTATE_H
#define WASMINT_INSTRUCTIONSTATE_H

#include <vector>
#include <Variable.h>
#include <instructions/Instruction.h>


class InstructionState {

    uint32_t state_ = 0;
    InstructionState* childInstruction = nullptr;
    Variable result_;
    bool finished_ = false;
    std::vector<Variable> results;
    Instruction* instruction_;


public:
    InstructionState(Instruction* instruction) {
    }

    Variable result() {
        return result_;
    }

    void step(Thread& thread) {
        if (childInstruction != nullptr) {
            childInstruction->step(thread);
            if (childInstruction->finished()) {
                results.push_back(childInstruction->result());
                delete childInstruction;
                childInstruction = nullptr;
            }
        } else {
            instruction_->execute(thread);
        }
    }

    bool finished() {
        return finished_;
    }

    uint32_t state() {
        return state_;
    }

    void state(uint32_t newState) {
        state_ = newState;
    }

    InstructionState& getChildOrThis() {
        if (childInstruction != nullptr)
            return *childInstruction;
        return *this;
    }

};


#endif //WASMINT_INSTRUCTIONSTATE_H

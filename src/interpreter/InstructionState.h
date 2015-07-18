#ifndef WASMINT_INSTRUCTIONSTATE_H
#define WASMINT_INSTRUCTIONSTATE_H

#include <vector>
#include <Variable.h>
#include <instructions/StepResult.h>

class Instruction;
class Thread;

class InstructionState {

    uint32_t state_ = 0;
    InstructionState* childInstruction = nullptr;
    Variable result_;
    bool finished_ = false;
    std::vector<Variable> results_;
    Instruction* instruction_;


public:
    InstructionState(Instruction* instruction) : instruction_(instruction) {
    }

    virtual ~InstructionState();

    Variable result() {
        return result_;
    }

    Signal step(Thread& thread);

    bool finished() {
        return finished_;
    }

    Instruction* instruction() {
        return instruction_;
    }

    uint32_t state() {
        return state_;
    }

    std::vector<Variable>& results() {
        return results_;
    }

    void state(uint32_t newState) {
        state_ = newState;
    }

    InstructionState& getChildOrThis() {
        if (childInstruction != nullptr)
            return childInstruction->getChildOrThis();
        return *this;
    }

    void clearResults() {
        results_.clear();
    }

};


#endif //WASMINT_INSTRUCTIONSTATE_H

#ifndef WASMINT_STEPRESULT_H
#define WASMINT_STEPRESULT_H

#include <Variable.h>

class Instruction;

enum class Signal {
    None,
    Break,
    Continue,
    Return
};

class StepResult {
    Instruction* newChildInstruction_ = nullptr;
    Variable result_;
    Signal signal_ = Signal::None;

public:
    StepResult(Instruction* newChildInstruction) : newChildInstruction_(newChildInstruction) {
    }
    StepResult(Variable& result) : result_(result){
    }
    StepResult(Signal signal, Variable result = Variable())
    : signal_(signal), result_(result) {
    }
    StepResult() {

    }

    Variable result() {
        return result_;
    }

    Instruction* newChildInstruction() {
        return newChildInstruction_;
    }

    Signal signal() {
        return signal_;
    }
};

#endif //WASMINT_STEPRESULT_H

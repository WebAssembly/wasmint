#ifndef WASMINT_STEPRESULT_H
#define WASMINT_STEPRESULT_H

#include <Variable.h>

class Instruction;

class StepResult {
    Instruction* newChildInstruction_ = nullptr;
    Variable result_;

public:
    StepResult(Instruction* newChildInstruction) : newChildInstruction_(newChildInstruction) {
    }
    StepResult(Variable& result) : result_(result){
    }
    StepResult() {

    }

    Variable result() {
        return result_;
    }

    Instruction* newChildInstruction() {
        return newChildInstruction_;
    }
};

#endif //WASMINT_STEPRESULT_H

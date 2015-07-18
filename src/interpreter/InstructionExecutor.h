#ifndef WASMINT_INSTRUCTIONEXECUTOR_H
#define WASMINT_INSTRUCTIONEXECUTOR_H


#include <interpreter/StepResult.h>
#include "Thread.h"

ExceptionMessage(UnknownInstruction)

class InstructionExecutor {

public:
    static StepResult execute(Instruction& instruction, Thread& thread);
    static bool handleSignal(Instruction& instruction, InstructionState& currentState, Signal signal);
};


#endif //WASMINT_INSTRUCTIONEXECUTOR_H

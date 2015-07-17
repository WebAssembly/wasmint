

#ifndef WASMINT_FOREVER_H
#define WASMINT_FOREVER_H


#include <instructions/Instruction.h>
#include "Break.h"
#include "Continue.h"

class Forever : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Void::instance()};
    }

    virtual std::string name() {
        return "forever";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual bool handleSignal(InstructionState& currentState, Signal signal) {
        if (signal == Signal::Break) {
            currentState.state(10);
            return true;
        }
        return signal == Signal::Continue;
    }

    virtual StepResult execute(Thread &thread) {
        if (thread.getInstructionState().state() >= 10) {
            return StepResult();
        }
        thread.getInstructionState().clearResults();
        thread.getInstructionState().state(0);
        return StepResult(children().front());
    }
};



#endif //WASMINT_FOREVER_H

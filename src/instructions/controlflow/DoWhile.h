

#ifndef WASMINT_DOWHILE_H
#define WASMINT_DOWHILE_H


#include <instructions/Instruction.h>
#include <types/Int32.h>
#include "Break.h"
#include "Continue.h"

class DoWhile : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Void::instance(), Int32::instance()};
    }

    virtual std::string name() {
        return "do_while";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual bool handleSignal(InstructionState& currentState, Signal signal) {
        if (signal == Signal::Break) {
            currentState.state(10);
        }
        return signal == Signal::Continue;
    }


    virtual StepResult execute(Thread &thread) {
        InstructionState& state = thread.getInstructionState();
        switch(state.state()) {
            case 0:
            case 1:
                return children().at(0);
            case 2:
                return children().at(1);
            default:
                if (Int32::getValue(state.results().back()) != 0) {
                    state.state(0);
                } else {
                    return StepResult();
                }
        }
    }
};



#endif //WASMINT_DOWHILE_H



#ifndef WASMINT_OPCODE_H
#define WASMINT_OPCODE_H


#include <cstdint>
#include <vector>

#include "types/Type.h"
#include "Variable.h"
#include "StepResult.h"
#include <interpreter/Thread.h>
#include <interpreter/RuntimeEnvironment.h>

class InstructionState;

class Instruction {

    std::vector<Instruction*> children_;

public:
    virtual ~Instruction() {
        for(Instruction* child : children()) {
            delete child;
        }
    }

    virtual void children(std::vector<Instruction*> newChildren) {
        children_ = newChildren;
    }
    std::vector<Instruction*> children() {
        return children_;
    }

    virtual std::string name() = 0;

    virtual std::vector<Type*> childrenTypes() = 0;
    virtual Type* returnType() = 0;

    virtual StepResult execute(Thread &thread) = 0;

    virtual bool handleSignal(InstructionState& currentState, Signal signal);
};


#endif //WASMINT_OPCODE_H

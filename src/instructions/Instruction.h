

#ifndef WASMINT_OPCODE_H
#define WASMINT_OPCODE_H


#include <cstdint>
#include <vector>

#include "types/Type.h"
#include "Variable.h"
#include <interpreter/Thread.h>
#include <interpreter/RuntimeEnvironment.h>

class StepResult {
    Instruction* newChildInstruction = nullptr;
    Variable result;
};

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

    virtual Variable execute(Thread &thread) = 0;
};


#endif //WASMINT_OPCODE_H

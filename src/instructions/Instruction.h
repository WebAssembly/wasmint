

#ifndef WASMINT_OPCODE_H
#define WASMINT_OPCODE_H


#include <cstdint>
#include <vector>
#include <interpreter/RuntimeEnvironment.h>

#include "types/Type.h"
#include "Variable.h"

class Instruction {

    std::vector<Instruction*> children_;

public:
    virtual void children(std::vector<Instruction*> newChildren) {
        children_ = newChildren;
    }
    std::vector<Instruction*> children() {
        return children_;
    }

    virtual std::string name() = 0;

    virtual std::vector<Type*> childrenTypes() = 0;
    virtual Type* returnType() = 0;

    virtual Variable execute(RuntimeEnvironment & env) = 0;

};


#endif //WASMINT_OPCODE_H

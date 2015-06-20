//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_LITERAL_H
#define WASMINT_LITERAL_H


#include "Instruction.h"

class Literal : public Instruction {
    Variable value;

public:
    Literal(ByteStream& stream, ModuleContext& context) {
        uint32_t typeId = stream.popLEB128();
        // TODO finish me
    }

    virtual std::vector<Type*> childrenTypes() {
        return {};
    }

    virtual std::string name() {
        return "literal";
    }

    virtual Type* returnType() {
        return &value.type();
    }

    virtual Variable execute(Environment& env) {
        return value;
    }
};


#endif //WASMINT_LITERAL_H

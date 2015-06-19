//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_SETLOCAL_H
#define WASMINT_SETLOCAL_H

#include <cstdint>
#include <Instruction.h>
#include <parsing/ByteStream.h>

class SetLocal : public Instruction {

    uint32_t localIndex;
    uint32_t value;

public:
    SetLocal(ByteStream& stream) {
        localIndex = stream.popLEB128();
        value = stream.popLEB128();
    }

    virtual std::vector<Type> childrenTypes() {
        return {};
    }

    virtual Type returnType() {
        return Type::Int32();
    }

    virtual Variable execute(Environment& env) {
        env.variable(localIndex).int32Value(value);
        return Variable();
    }
};


#endif //WASMINT_SETLOCAL_H

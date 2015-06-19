//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_SETLOCAL_H
#define WASMINT_SETLOCAL_H

#include <cstdint>
#include <Instruction.h>
#include <parsing/ByteStream.h>
#include <types/Int32.h>

class SetLocal : public Instruction {

    uint32_t localIndex;
    int32_t value;

public:
    SetLocal(ByteStream& stream) {
        localIndex = stream.popLEB128();
        value = stream.popLEB128();
    }

    virtual std::vector<Type*> childrenTypes() {
        return {};
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Environment& env) {
        Int32::setValue(env.variable(localIndex), value);
        return Variable();
    }
};


#endif //WASMINT_SETLOCAL_H

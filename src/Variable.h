//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_VARIABLE_H
#define WASMINT_VARIABLE_H

#include <cstdint>
#include <types/Void.h>
#include "types/Type.h"

class Variable {
    Type* type_;
    void* value_ = nullptr;

public:
    Variable(Type* type = Void::instance()) : type_(type) {
        value_ = new uint8_t[type->size()];
    }

    Type& type() {
        return *type_;
    }

    void* value() {
        return value_;
    }
};


#endif //WASMINT_VARIABLE_H



#ifndef WASMINT_VARIABLE_H
#define WASMINT_VARIABLE_H

#include <cstdint>
#include <types/Void.h>
#include <string.h>
#include "types/Type.h"
#include <vector>

class Variable {
    Type* type_;
    std::vector<uint8_t> value_;

public:
    Variable(Type* type = Void::instance()) : type_(type) {
        value_.resize(type->size());
    }

    Type& type() {
        return *type_;
    }

    void* value() {
        return (void*) value_.data();
    }
};


#endif //WASMINT_VARIABLE_H

//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_VARIABLE_H
#define WASMINT_VARIABLE_H

#include <cstdint>
#include "Type.h"

class Variable {
    Type type_;
    int32_t  value_;
public:
    Variable(Type type, int32_t intValue) : type_(type), value_(intValue) {

    }

    void int32Value(int32_t value) {
        value_ = value;
    }

    int32_t int32Value() {
        return value_;
    }

    Type type() {
        return type_;
    }
};


#endif //WASMINT_VARIABLE_H

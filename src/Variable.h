

#ifndef WASMINT_VARIABLE_H
#define WASMINT_VARIABLE_H

#include <cstdint>
#include <vector>

class InvalidDataSize : public std::exception {};

class Type;

class Variable {
    Type* type_;
    std::vector<uint8_t> value_;

public:
    Variable();
    Variable(Type* type);

    Type& type() {
        return *type_;
    }

    void* value() {
        return (void*) value_.data();
    }

    std::vector<uint8_t> data() {
        return value_;
    }

    void setValue(std::vector<uint8_t> newData);
};


#endif //WASMINT_VARIABLE_H

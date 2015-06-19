//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_TYPE_H
#define WASMINT_TYPE_H


#include <cstdint>

class Type {

    uint8_t id_;

    static Type Int32_;
    static Type Void_;

    Type(uint8_t id) : id_(id) {

    }

public:
    Type(const Type& type) : id_(type.id_) {

    }

    static const Type& Int32() {
        return Int32_;
    }

    static const Type& Void() {
        return Void_;
    }

    bool operator==(const Type& other) const {
        return this->id_ == other.id_;
    }

    bool operator!=(const Type& other) const {
        return this->id_ != other.id_;
    }

};


#endif //WASMINT_TYPE_H

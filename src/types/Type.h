//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_TYPE_H
#define WASMINT_TYPE_H


#include <cstdint>
#include <parsing/ByteStream.h>
#include <typeinfo>

class IncompatibleType : public std::exception {};

class Type {

protected:
    Type() {

    }

public:
    Type(const Type& type) {

    }

    virtual std::string name() = 0;

    virtual void parse(ByteStream& stream, void* data, std::size_t dataLength) = 0;

    virtual std::size_t size() = 0;

    bool operator==(const Type& other) const {
        return typeid(*this) == typeid(other);
    }

    bool operator!=(const Type& other) const {
        return typeid(*this) != typeid(other);
    }

};


#endif //WASMINT_TYPE_H

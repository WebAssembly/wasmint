

#ifndef WASMINT_TYPE_H
#define WASMINT_TYPE_H


#include <cstdint>
#include <parsing/ByteStream.h>
#include <typeinfo>
#include <Variable.h>

class IncompatibleType : public std::exception {};

class Type {

protected:
    Type() {

    }

public:
    Type(const Type& type) {

    }

    virtual Type* localType() {
        return this;
    }

    virtual std::vector<uint8_t> convertFromMemoryType(std::vector<uint8_t> bytes, Type* memoryType) {
        return bytes;
    }

    virtual std::string name() = 0;

    virtual void parse(ByteStream& stream, void* data) = 0;

    virtual std::size_t size() = 0;

    bool operator==(const Type& other) const {
        return typeid(*this) == typeid(other);
    }

    bool operator!=(const Type& other) const {
        return typeid(*this) != typeid(other);
    }

};


#endif //WASMINT_TYPE_H

//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_INT64_H
#define WASMINT_INT64_H


#include <Type.h>
#include <Variable.h>

class Int64  : public Type {

protected:
    Int64() {
    }


public:
    static Int64* instance() {
        static Int64* instance = new Int64();
        return instance;
    }

    virtual void parse(ByteStream& stream, void* data, std::size_t dataLength) {
        int64_t value = stream.popLEB128() << 32; // FIXME
        value |= stream.popLEB128();
        (*(int64_t*)data) = value;
    }

    static int64_t getValue(Variable variable) {
        if (variable.type() == *instance()) {
            int64_t result = 0;
            int64_t* data = (int64_t*) variable.value();
            result = *data;
            return result;
        } else {
            throw IncompatibleType();
        }
    }

    static void setValue(Variable variable, int32_t value) {
        if (variable.type() == *instance()) {
            int32_t* data = (int32_t*) variable.value();
            (*data) = value;
        } else {
            throw IncompatibleType();
        }
    }

    virtual std::size_t size() {
        return 8;
    }

};


#endif //WASMINT_INT64_H

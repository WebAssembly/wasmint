//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_INT32_H
#define WASMINT_INT32_H


#include <types/Type.h>
#include <Variable.h>

class Int32 : public Type {

protected:
    Int32() {
    }


public:
    static Int32* instance() {
        static Int32* instance = new Int32();
        return instance;
    }

    virtual std::string name() {
        return "int32";
    }

    virtual void parse(ByteStream& stream, void* data, std::size_t dataLength) {
        uint32_t value = stream.popLEB128();
        (*(uint32_t*)data) = value;
    }

    static int32_t getValue(Variable variable) {
        if (variable.type() == *instance()) {
            int32_t result = 0;
            int32_t* data = (int32_t*) variable.value();
            result = *data;
            return result;
        } else {
            throw IncompatibleType();
        }
    }

    static void setValue(Variable& variable, int32_t value) {
        if (variable.type() == *instance()) {
            int32_t* data = (int32_t*) variable.value();
            (*data) = value;
        } else {
            throw IncompatibleType();
        }
    }

    virtual std::size_t size() {
        return 4;
    }

};


#endif //WASMINT_INT32_H

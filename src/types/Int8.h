//
// Created by teemperor on 22.06.15.
//

#ifndef WASMINT_INT8_H
#define WASMINT_INT8_H

#include <string>
#include <types/Type.h>
#include <Variable.h>
#include "Int32.h"

class Int8 : public Type {

protected:
    Int8() {
    }

public:
    static Int8* instance() {
        static Int8 instance;
        return &instance;
    }

    virtual std::string name() {
        return "int8";
    }

    virtual Type* localType() override {
        return Int32::instance();
    }

    static int8_t getFromStream(ByteStream& stream) {
        int8_t result = 0;
        uint8_t shift = 0;
        uint8_t size = 8;

        uint8_t byte;

        while(true) {
            byte = stream.popChar();

            result |= ((byte & 0x7F) << shift);
            shift += 7;

            if ((byte & 0x80u) == 0u)
                break;
        }

        /* sign bit of byte is second high order bit (0x40) */
        if ((shift < size) && ((0x40 & byte) != 0))
            /* sign extend */
            result |= - (1 << shift);

        return result;
    }

    virtual void parse(ByteStream& stream, void* data) {
        int8_t value = getFromStream(stream);
        (*(int8_t*)data) = value;
    }

    static int8_t getValue(Variable variable) {
        if (variable.type() == *instance()) {
            int8_t result = 0;
            int8_t* data = (int8_t*) variable.value();
            result = *data;
            return result;
        } else {
            throw IncompatibleType();
        }
    }

    static void setValue(Variable& variable, int8_t value) {
        if (variable.type() == *instance()) {
            int8_t* data = (int8_t*) variable.value();
            (*data) = value;
        } else {
            throw IncompatibleType();
        }
    }

    virtual std::size_t size() {
        return 1;
    }

};



#endif //WASMINT_INT8_H



#ifndef WASMINT_FLOAT32_H
#define WASMINT_FLOAT32_H


#include <types/Type.h>
#include <Variable.h>

class Float32  : public Type {

protected:
    Float32() {
    }


public:
    static Float32* instance() {
        static Float32 instance;
        return &instance;
    }

    virtual std::string name() {
        return "float32";
    }

    virtual void parse(ByteStream& stream, void* data) {
        ((uint8_t*) data)[0] = stream.popChar();
        ((uint8_t*) data)[1] = stream.popChar();
        ((uint8_t*) data)[2] = stream.popChar();
        ((uint8_t*) data)[3] = stream.popChar();
    }

    static float getValue(Variable variable) {
        if (variable.type() == *instance()) {
            float result = 0;
            float* data = (float*) variable.value();
            result = *data;
            return result;
        } else {
            throw IncompatibleType();
        }
    }

    static void setValue(Variable variable, float value) {
        if (variable.type() == *instance()) {
            float* data = (float*) variable.value();
            (*data) = value;
        } else {
            throw IncompatibleType();
        }
    }

    virtual std::size_t size() {
        return 4;
    }
};


#endif //WASMINT_FLOAT32_H

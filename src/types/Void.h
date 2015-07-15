

#ifndef WASMINT_VOID_H
#define WASMINT_VOID_H


#include <types/Type.h>

class NoVoidLiteralsSupported : public std::exception {};

class Void : public Type {

protected:
    Void() {
    }

public:
    static Void* instance() {
        static Void instance;
        return &instance;
    }

    virtual std::string name() {
        return "void";
    }

    virtual void parse(ByteStream& stream, void* data) {
        throw NoVoidLiteralsSupported();
    }

    virtual std::size_t size() {
        return 1;
    }

};


#endif //WASMINT_VOID_H

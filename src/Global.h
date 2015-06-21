//
// Created by teemperor on 21.06.15.
//

#ifndef WASMINT_GLOBAL_H
#define WASMINT_GLOBAL_H

#include <string>
#include <types/Type.h>

class Global {
    std::string name_;
    Type* type_;
public:
    Global(std::string name, Type* type) : name_(name), type_(type) {

    }

    std::string name() {
        return name_;
    }

    Type* type() {
        return type_;
    }
};


#endif //WASMINT_GLOBAL_H

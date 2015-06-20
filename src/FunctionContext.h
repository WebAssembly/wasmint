//
// Created by teemperor on 20.06.15.
//

#ifndef WASMINT_FUNCTIONCONTEXT_H
#define WASMINT_FUNCTIONCONTEXT_H


#include "types/Type.h"
#include <vector>

class FunctionContext {

    std::vector<Type*> locals_;

public:
    FunctionContext() {
    }

    FunctionContext(std::vector<Type*> locals) : locals_(locals) {
    }

    std::vector<Type*> locals() {
        return locals_;
    }
};


#endif //WASMINT_FUNCTIONCONTEXT_H

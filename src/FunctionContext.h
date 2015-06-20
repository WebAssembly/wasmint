

#ifndef WASMINT_FUNCTIONCONTEXT_H
#define WASMINT_FUNCTIONCONTEXT_H


#include "types/Type.h"
#include <vector>

/**
 * The context of a function. This contains all information that are needed to
 * create the instructions in the AST of the related function.
 */
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

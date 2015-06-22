

#ifndef WASMINT_FUNCTIONCONTEXT_H
#define WASMINT_FUNCTIONCONTEXT_H


#include "types/Type.h"
#include "FunctionSignature.h"
#include <vector>

/**
 * The context of a function. This contains all information that are needed to
 * create the instructions in the AST of the related function.
 */
class FunctionContext : public FunctionSignature {

    std::vector<Type*> locals_;

public:
    FunctionContext() {
    }

    FunctionContext(std::string name, Type* returnType, std::vector<Type*> parameterTypes,
                    std::vector<Type*> locals, bool exported)
            : locals_(locals), FunctionSignature(name, returnType, parameterTypes, exported) {

    }

    std::vector<Type*> pureLocals() {
        return locals_;
    }

    std::vector<Type*> locals() {
        std::vector<Type*> result;

        for(Type* type : parameters()) {
            result.push_back(type);
        }

        for(Type* type : pureLocals()) {
            result.push_back(type);
        }
        return result;
    }
};


#endif //WASMINT_FUNCTIONCONTEXT_H

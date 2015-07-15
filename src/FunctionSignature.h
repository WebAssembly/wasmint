

#ifndef WASMINT_FUNCTIONSIGNATURE_H
#define WASMINT_FUNCTIONSIGNATURE_H

#include <string>
#include <vector>
#include <types/Void.h>

#include "types/Type.h"
#include "Variable.h"

/**
 * Contains all information that are relevant to an caller of the given function.
 */
class FunctionSignature {

    std::string name_;
    Type* returnType_ = Void::instance();
    std::vector<Type*> parameterTypes_;
    bool isExported_ = false;

public:
    FunctionSignature() {

    }

    FunctionSignature(std::string name, Type* returnType,
                      std::vector<Type*> parameterTypes, bool exported)
    : name_(name), returnType_(returnType), parameterTypes_(parameterTypes),
      isExported_(exported)
    {
    }

    Type* returnType() {
        return returnType_;
    }

    std::vector<Type*> parameters() {
        return parameterTypes_;
    }


    std::string name() {
        return name_;
    }

    bool isExported() {
        return isExported_;
    }
};


#endif //WASMINT_FUNCTIONSIGNATURE_H

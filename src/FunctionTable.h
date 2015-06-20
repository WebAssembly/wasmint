

#ifndef WASMINT_FUNCTIONTABLE_H
#define WASMINT_FUNCTIONTABLE_H

#include <cstdint>
#include <string>
#include <map>
#include "FunctionSignature.h"
#include "ExceptionWithMessage.h"

ExceptionMessage(UnknownLocalFunctionId)

/**
 * Maps local function ids to function signatures.
 */
class FunctionTable {

    std::vector<FunctionSignature> functionNames_;

public:
    FunctionTable() {
    }

    void addFunctionSignature(FunctionSignature signature) {
        functionNames_.push_back(signature);
    }

    FunctionSignature getFunctionSignature(uint32_t localFunctionId) {
        if (localFunctionId > functionNames_.size())
            throw UnknownLocalFunctionId(std::to_string(localFunctionId));
        return functionNames_[localFunctionId];
    }
};


#endif //WASMINT_FUNCTIONTABLE_H

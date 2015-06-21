

#ifndef WASMINT_FUNCTION_H
#define WASMINT_FUNCTION_H

#include <string>
#include <vector>

#include "types/Type.h"
#include "Variable.h"
#include "FunctionSignature.h"
#include "FunctionContext.h"
#include <memory>
#include <interpreter/RuntimeEnvironment.h>

class Instruction;

class Function : public FunctionContext {

    /**
     * The AST of this function which contains all instructions of this function.
     */
    Instruction* mainInstruction_;

public:
    Function(FunctionContext& context, Instruction* mainInstruction);
    virtual ~Function();

    Variable execute(RuntimeEnvironment & environment);
};


#endif //WASMINT_FUNCTION_H

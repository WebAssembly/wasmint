

#ifndef WASMINT_FUNCTION_H
#define WASMINT_FUNCTION_H

#include <string>
#include <vector>

#include "types/Type.h"
#include "Variable.h"
#include "FunctionSignature.h"
#include "FunctionContext.h"

class Instruction;
class RuntimeEnvironment;


class Function : public FunctionContext {

    /**
     * The AST of this function which contains all instructions of this function.
     */
    Instruction* mainInstruction;

public:
    Function(FunctionContext& context, Instruction* mainInstruction)
        : FunctionContext(context), mainInstruction(mainInstruction)
    {

    }


    Variable execute(RuntimeEnvironment & environment);
};


#endif //WASMINT_FUNCTION_H



#include "Function.h"
#include <instructions/Instruction.h>

Function::Function(FunctionContext& context, Instruction* mainInstruction)
        : FunctionContext(context), mainInstruction_(mainInstruction) {
}

Function::~Function() {
    delete mainInstruction_;
}
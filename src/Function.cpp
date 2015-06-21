

#include "Function.h"
#include <instructions/Instruction.h>

Variable Function::execute(RuntimeEnvironment & environment) {
    return mainInstruction_->execute(environment);
}

Function::Function(FunctionContext& context, Instruction* mainInstruction)
        : FunctionContext(context), mainInstruction_(mainInstruction) {
}

Function::~Function() {
    delete mainInstruction_;
}
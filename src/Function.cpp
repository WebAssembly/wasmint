

#include "Function.h"
#include <instructions/Instruction.h>

Variable Function::execute(Thread& thread) {
    return mainInstruction_->execute(thread);
}

Function::Function(FunctionContext& context, Instruction* mainInstruction)
        : FunctionContext(context), mainInstruction_(mainInstruction) {
}

Function::~Function() {
    delete mainInstruction_;
}
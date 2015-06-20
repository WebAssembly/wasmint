

#include "Function.h"
#include <interpreter/Environment.h>
#include <instructions/Instruction.h>

Variable Function::execute(Environment& environment) {
    return mainInstruction->execute(environment);
}
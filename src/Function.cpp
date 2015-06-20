

#include "Function.h"
#include <interpreter/RuntimeEnvironment.h>
#include <instructions/Instruction.h>

Variable Function::execute(RuntimeEnvironment & environment) {
    return mainInstruction->execute(environment);
}
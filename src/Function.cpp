//
// Created by teemperor on 19.06.15.
//

#include "Function.h"
#include <interpreter/Environment.h>
#include <instructions/Instruction.h>

Variable Function::execute(Environment& environment) {
    return mainInstruction->execute(environment);
}
//
// Created by teemperor on 19.06.15.
//

#include "Function.h"
#include <interpreter/Environment.h>
#include <Instruction.h>

void Function::execute(Environment& environment) {
    environment.enterFunction(*this);
    mainInstruction->execute(environment);
    environment.leaveFunction();
}
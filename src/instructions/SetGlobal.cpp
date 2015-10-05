//
// Created by teemperor on 21.06.15.
//

#include "SetGlobal.h"
#include <interpreter/MachineState.h>

Variable SetGlobal::execute(Thread& thread) {
    return thread.machineState().global(globalName) = children().at(0)->execute(thread);
}
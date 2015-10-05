//
// Created by teemperor on 21.06.15.
//

#include "GetGlobal.h"
#include <interpreter/MachineState.h>

Variable GetGlobal::execute(Thread& thread) {
    return thread.machineState().global(globalName);
}
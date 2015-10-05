

#include "Print.h"
#include <interpreter/MachineState.h>

Variable Print::execute(Thread& thread) {
    Variable v = children().at(0)->execute(thread);
    thread.machineState().print(std::to_string(Int32::getValue(v)));

    return Variable();
}
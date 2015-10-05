//
// Created by teemperor on 05.10.15.
//

#include <Global.h>
#include "MachineState.h"
#include <Module.h>
#include <Function.h>

void MachineState::useModule(Module& module) {
    std::vector<Function*> functions = module.functions();
    for(Function* function : functions) {
        functions_[function->name()] = function;
    }

    for(Global& global : module.globals()) {
        globals_[global.name()] = Variable(global.type());
    }
}

Function& MachineState::function(std::string name) {
    auto functionIterator = functions_.find(name);
    if (functionIterator != functions_.end()) {
        return *functionIterator->second;
    } else {
        throw NoFunctionWithName(name);
    }
}
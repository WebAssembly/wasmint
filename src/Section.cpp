

#include "Section.h"
#include <Function.h>

Function &Section::getFunction(std::string name) {
    for(Function* function : functions_) {
        if (function->name() == name) {
            return *function;
        }
    }
    throw NoFunctionWithThatName(name);
}

Section::~Section() {
    for(Function* function : functions_) {
        delete function;
    }
}
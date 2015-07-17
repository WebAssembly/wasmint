#include "FunctionState.h"

#include <Function.h>

FunctionState::FunctionState(Function& function) {
    for(Type* type : function.locals()) {
        variables_.push_back(Variable(type));
    }
}
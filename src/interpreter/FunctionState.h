#ifndef WASMINT_FUNCTIONSTATE_H
#define WASMINT_FUNCTIONSTATE_H


#include <Variable.h>

class Function;

class FunctionState {

    std::vector<Variable> variables_;
    
public:
    FunctionState() {
    }

    FunctionState(Function& function);

    Variable& variable(uint32_t index) {
        return variables_.at(index);
    }
};


#endif //WASMINT_FUNCTIONSTATE_H

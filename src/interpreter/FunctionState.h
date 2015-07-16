#ifndef WASMINT_FUNCTIONSTATE_H
#define WASMINT_FUNCTIONSTATE_H


#include <Variable.h>
#include <Function.h>

class FunctionState {

    std::vector<Variable> variables_;
    
public:
    FunctionState() {
    }
    FunctionState(Function& function, Instruction* mainInstruction) {
        for(Type* type : function.locals()) {
            variables_.push_back(Variable(type));
        }
    }

    Variable& variable(uint32_t index) {
        return variables_.at(index);
    }
};


#endif //WASMINT_FUNCTIONSTATE_H

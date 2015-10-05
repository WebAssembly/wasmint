//
// Created by teemperor on 05.10.15.
//

#ifndef WASMINT_THREAD_H
#define WASMINT_THREAD_H

#include <stack>
#include <Variable.h>
#include <ExceptionWithMessage.h>

ExceptionMessage(StackLimitReached)
ExceptionMessage(IllegalUseageOfBreak)
ExceptionMessage(IllegalUseageOfContinue)


class Function;
class MachineState;
class Heap;


class Thread {

    MachineState* machineState_ = nullptr;

    /**
     * The stack containing the local variables. The indices of the variables in each vector are equal to their
     * local indices as used by get_local and set_local.
     */
    std::stack<std::vector<Variable>> stack;


    uint32_t stackLimit = 50;

    void createLocals(std::vector<Type*> variableTypes);

public:
    Thread(MachineState* machineState = nullptr) : machineState_(machineState) {
    }

    Variable& variable(uint32_t index) {
        return stack.top().at(index);
    }

    Heap & heap();

    void enterFunction(Function& function);


    /**
     * Leave the last entered function
     */
    void leaveFunction() {
        stack.pop();
    }

    MachineState& machineState() {
        return *machineState_;
    }


    Variable callFunction(std::string functionName, std::vector<Variable> parameters = std::vector<Variable>());

};


#endif //WASMINT_THREAD_H

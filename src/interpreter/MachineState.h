//
// Created by teemperor on 05.10.15.
//

#ifndef WASMINT_MACHINESTATE_H
#define WASMINT_MACHINESTATE_H

#include "Thread.h"
#include "Heap.h"
#include <vector>
#include <map>

ExceptionMessage(NoGlobalWithName);
ExceptionMessage(NoFunctionWithName)

class Function;
class Module;

class MachineState {

    /**
     * The current heap.
     */
    Heap heap_;


    /**
     * All functions that are accessible with the currently loaded modules.
     * The keys are the function names.
     */
    std::map<std::string, Function*> functions_;

    /**
     * All globals that are accessible with the currently loaded modules.
     * The keys are the variable names.
     */
    std::map<std::string, Variable> globals_;

    std::vector<Thread> threads_;

    /**
     * The stdout of this program. We currently just append to this string and then read it via stdou().
     */
    std::string stdout_;

public:
    MachineState() {
    }

    Heap& heap() {
        return heap_;
    }

    Thread& createThread() {
        threads_.push_back(Thread(this));
        return *threads_.end();
    }

    std::vector<Thread> threads() {
        return threads_;
    }

    Variable& global(std::string name) {
        auto globalIterator = globals_.find(name);
        if (globalIterator != globals_.end()) {
            return globalIterator->second;
        } else {
            throw NoGlobalWithName(name);
        }
    }

    Function & function(std::string name);

    void print(std::string s) {
        stdout_ += s;
    }

    std::string stdout() {
        return stdout_;
    }

    void useModule(Module& module);

};


#endif //WASMINT_MACHINESTATE_H

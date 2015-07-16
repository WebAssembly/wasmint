

#ifndef WASMINT_ENVIRONMENT_H
#define WASMINT_ENVIRONMENT_H


#include <Variable.h>
#include <vector>
#include <memory>
#include <stack>
#include <map>
#include <Module.h>
#include "Heap.h"
#include "Thread.h"

ExceptionMessage(NoFunctionWithName)
ExceptionMessage(NoGlobalWithName)

class CalledBreak;
class CalledContinue;

/**
 * Contains all variable values during the interpretation of a program.
 * TODO: It currently also hosts the stack, which should be changed as soon as things get multithreaded...
 */
class RuntimeEnvironment {

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

    /**
     * The current heap.
     */
    Heap heap_;

    /**
     * The stdout of this program. We currently just append to this string and then read it via stdou().
     */
    std::string stdout_;

    // FIXME Use smart pointers if possible...
    std::vector<Thread*> threads_;

public:
    RuntimeEnvironment() : heap_(1024) {
    }

    virtual ~RuntimeEnvironment() {
        for(Thread* thread : threads_) {
            delete thread;
        }
    }

    Thread& createThread() {
        Thread* newThread = new Thread(*this);
        threads_.push_back(newThread);
        return *threads_.back();
    }

    void useModule(Module& module);

    Heap& heap() {
        return heap_;
    }

    Variable& global(std::string name) {
        auto globalIterator = globals_.find(name);
        if (globalIterator != globals_.end()) {
           return globalIterator->second;
        } else {
            throw NoGlobalWithName(name);
        }
    }

    void print(std::string s) {
        stdout_ += s;
    }

    std::string stdout() {
        return stdout_;
    }

    std::map<std::string, Function*>& functions() {
        return functions_;
    }

    /**
     * All globals that are accessible with the currently loaded modules.
     * The keys are the variable names.
     */
    std::map<std::string, Variable>& globals() {
        return globals_;
    }
};


#endif //WASMINT_ENVIRONMENT_H

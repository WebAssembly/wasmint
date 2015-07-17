
#include "RuntimeEnvironment.h"
#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include <Function.h>
#include "Thread.h"

void RuntimeEnvironment::useModule(Module& module) {
    std::vector<Function*> functions = module.functions();
    for(Function* function : functions) {
        functions_[function->name()] = function;
    }

    for(Global& global : module.globals()) {
        globals_[global.name()] = Variable(global.type());
    }

}

Thread &RuntimeEnvironment::createThread() {
    Thread* newThread = new Thread(*this);
    threads_.push_back(newThread);
    return *threads_.back();
}

RuntimeEnvironment::~RuntimeEnvironment() {
    for(Thread* thread : threads_) {
        delete thread;
    }
}
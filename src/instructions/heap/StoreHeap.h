

#ifndef WASMINT_STOREHEAP_H
#define WASMINT_STOREHEAP_H

#include <Instruction.h>

class StoreHeap : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance()};
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Thread& thread) {
    }
};


#endif //WASMINT_STOREHEAP_H

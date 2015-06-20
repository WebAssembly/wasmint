//
// Created by teemperor on 20.06.15.
//

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

    virtual Variable execute(Environment& env) {
    }
};


#endif //WASMINT_STOREHEAP_H

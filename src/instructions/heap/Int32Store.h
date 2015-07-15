

#ifndef WASMINT_STOREHEAP_H
#define WASMINT_STOREHEAP_H


#include <instructions/Instruction.h>
#include <types/Int32.h>

class Int32Store : public Instruction {

public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance(), Int32::instance()};
    }

    virtual Type* returnType() {
        return Int32::instance();
    }

    virtual std::string name() {
        return "int32.store";
    }

    virtual Variable execute(RuntimeEnvironment& env) {
        uint32_t offset = static_cast<uint32_t>(Int32::getValue(children().at(0)->execute(env)));

        Variable value = children().at(1)->execute(env);
        env.heap().setBytes(offset, value.data());


        return value;
    }
};


#endif //WASMINt_STOREHEAP_H



#ifndef WASMINT_LOADHEAP_H
#define WASMINT_LOADHEAP_H


#include <instructions/Instruction.h>
#include <types/Int32.h>

class Int32Load : public Instruction {

public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance()};
    }

    virtual Type* returnType() {
        return Int32::instance();
    }

    virtual std::string name() {
        return "int32.load";
    }

    virtual Variable execute(Thread &thread) {
        uint32_t offset = static_cast<uint32_t>(Int32::getValue(children().at(0)->execute(thread)));

        std::vector<uint8_t> bytes = thread.runtimeEnvironment().heap().getBytes(offset, Int32::instance()->size());

        Variable result = Variable(Int32::instance()->localType());
        result.setValue(bytes);

        return result;
    }
};


#endif //WASMINT_LOADHEAP_H

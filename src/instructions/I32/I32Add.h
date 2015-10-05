

#ifndef WASMINT_I32ADD_H
#define WASMINT_I32ADD_H


#include <types/Int32.h>
#include "instructions/Instruction.h"

class I32Add : public Instruction {

public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance(), Int32::instance()};
    }

    virtual std::string name() {
        return "int32.add";
    }


    virtual Type* returnType() {
        return Int32::instance();
    }

    virtual Variable execute(Thread& thread) {
        int32_t left = Int32::getValue(children().at(0)->execute(thread));
        int32_t right = Int32::getValue(children().at(1)->execute(thread));

        Variable result = Variable(Int32::instance());
        Int32::setValue(result, left + right);
        return result;
    }
};


#endif //WASMINT_I32ADD_H

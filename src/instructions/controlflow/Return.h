

#ifndef WASMINT_RETURN_H
#define WASMINT_RETURN_H


#include <instructions/Instruction.h>
#include <types/Int32.h>

class CalledReturn {
public:
    Variable result;
};

class Return : public Instruction {
public:
    virtual std::vector<Type*> childrenTypes() {
        return {Int32::instance()};
    }

    virtual std::string name() {
        return "return";
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Environment& env) {
        CalledReturn returnCall;
        returnCall.result = children().at(0)->execute(env);
        throw returnCall;
    }
};



#endif //WASMINT_RETURN_H



#ifndef WASMINT_GETLOCAL_H
#define WASMINT_GETLOCAL_H


#include <instructions/Instruction.h>
#include <parsing/ByteStream.h>

class GetLocal : public Instruction {

    uint32_t localIndex;
    Type* returnType_;

public:
    GetLocal(ByteStream& stream, FunctionContext& context) {
        localIndex = stream.popULEB128();
        returnType_ = context.locals().at(localIndex);
    }

    virtual std::string name() {
        return "get_local";
    }

    virtual std::vector<Type*> childrenTypes() {
        return {};
    }

    virtual Type* returnType() {
        return returnType_;
    }

    virtual Variable execute(RuntimeEnvironment & env) {
        return env.variable(localIndex);
    }
};


#endif //WASMINT_GETLOCAL_H

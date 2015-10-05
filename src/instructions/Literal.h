

#ifndef WASMINT_LITERAL_H
#define WASMINT_LITERAL_H


#include <ModuleContext.h>
#include "Instruction.h"

class Literal : public Instruction {
    Variable literalValue;

public:
    Literal(ByteStream& stream, ModuleContext& context) {
        uint32_t typeId = stream.popULEB128();

        Type* type = context.typeTable().getType(typeId);

        literalValue = Variable(type);
        type->parse(stream, literalValue.value(), type->size());

    }

    virtual std::vector<Type*> childrenTypes() {
        return {};
    }

    virtual std::string name() {
        return "literal";
    }

    virtual Type* returnType() {
        return &literalValue.type();
    }

    virtual Variable execute(Thread& thread) {
        return literalValue;
    }
};


#endif //WASMINT_LITERAL_H

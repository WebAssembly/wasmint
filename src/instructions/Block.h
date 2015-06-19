//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_BLOCK_H
#define WASMINT_BLOCK_H


#include <Instruction.h>
#include <parsing/ByteStream.h>

class Block : public Instruction {

    uint32_t amountOfChildren;

public:
    Block(ByteStream& stream) {
        amountOfChildren = stream.popLEB128();
    }

    virtual std::vector<Type> childrenTypes() {
        std::vector<Type> result;
        for(uint32_t i = 0; i < amountOfChildren; i++) {
            result.push_back(Type::Int32());
        }
        return result;
    }

    virtual Type returnType() {
        return Type::Void();
    }

    virtual Variable execute(Environment& env) {
        for(Instruction* child : children()) {
            child->execute(env);
        }
        return Variable(Type::Void(), 0);
    }
};


#endif //WASMINT_BLOCK_H

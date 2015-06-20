//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_BLOCK_H
#define WASMINT_BLOCK_H


#include <instructions/Instruction.h>
#include <parsing/ByteStream.h>

class Block : public Instruction {

    uint32_t amountOfChildren;

public:
    Block(ByteStream& stream) {
        amountOfChildren = stream.popULEB128();
    }

    virtual std::string name() {
        return "block";
    }

    virtual std::vector<Type*> childrenTypes() {
        std::vector<Type*> result;
        for(uint32_t i = 0; i < amountOfChildren; i++) {
            result.push_back(Void::instance());
        }
        return result;
    }

    virtual Type* returnType() {
        return Void::instance();
    }

    virtual Variable execute(Environment& env) {
        for(Instruction* child : children()) {
            child->execute(env);
        }
        return Variable();
    }
};


#endif //WASMINT_BLOCK_H

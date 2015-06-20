//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_FUNCTIONPARSER_H
#define WASMINT_FUNCTIONPARSER_H


#include <Function.h>
#include <Variable.h>
#include <Instruction.h>
#include <instructions/InstructionSet.h>
#include <OpcodeTable.h>
#include <ModuleContext.h>
#include "ByteStream.h"

class FunctionParser {
    ByteStream& stream;
    std::vector<Type*> typeOfLocals;
    ModuleContext& context_;
    Instruction* mainInstruction;

protected:
    FunctionParser(ModuleContext& context, ByteStream& stream)
            : context_(context), stream(stream) {
    }

    void parse() {
        uint32_t numberOfLocals = stream.popLEB128();
        for (uint32_t i = 0; i < numberOfLocals; i++) {
            uint32_t typeData = stream.popLEB128();
            typeOfLocals.push_back(context_.typeTable().getType(typeData));
        }
        mainInstruction = parseInstruction();
    }

    Instruction* parseInstruction() {
        uint32_t opcode = stream.popLEB128();
        Instruction* instruction = InstructionSet::getInstruction(
                context_.opcodeTable().getInstruction(opcode), stream, context_);

        std::vector<Instruction*> children;

        for(uint32_t args = 0; args < instruction->childrenTypes().size(); args++) {
            Instruction* child = parseInstruction();
            children.push_back(child);
        }

        instruction->children(children);
        return instruction;
    }

    Function getParsedFunction(std::string name, Type* returnType, std::vector<Type*> parameters) {
        return Function(name, returnType, parameters, typeOfLocals, mainInstruction);
    }

public:
    static Function parse(ModuleContext& context, FunctionSignature& signature, ByteStream& stream) {
        FunctionParser parser(context, stream);
        parser.parse();
        return parser.getParsedFunction(signature.name(), signature.returnType(), signature.parameters());
    }
};


#endif //WASMINT_FUNCTIONPARSER_H

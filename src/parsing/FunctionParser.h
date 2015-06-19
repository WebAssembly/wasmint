//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_FUNCTIONPARSER_H
#define WASMINT_FUNCTIONPARSER_H


#include <Function.h>
#include <Variable.h>
#include <Instruction.h>
#include <InstructionSet.h>
#include <OpcodeTable.h>
#include <ModuleContext.h>
#include "ByteStream.h"

class FunctionParser {
    ByteStream& stream;
    std::vector<Type> typeOfLocals;
    ModuleContext& context_;
    Instruction* mainInstruction;

protected:
    FunctionParser(ModuleContext& context, ByteStream& stream)
            : context_(context), stream(stream) {
    }

    void parse() {
        uint32_t numberOfLocals = stream.popLEB128();
        for (uint32_t i = 0; i < numberOfLocals; i++) {
            typeOfLocals.push_back(Type::Int32());
            stream.popLEB128(); // TODO use the value
        }
        mainInstruction = parseInstruction();
    }

    Instruction* parseInstruction() {
        uint32_t opcode = stream.popLEB128();
        Instruction* instruction = InstructionSet::getInstruction(context_.opcodeTable().getInstruction(opcode), stream);

        std::vector<Instruction*> children;

        for(uint32_t args = 0; args < instruction->childrenTypes().size(); args++) {
            children.push_back(parseInstruction());
        }

        instruction->children(children);
        return instruction;
    }

    Function getParsedFunction(std::string name, Type returnType, std::vector<Type> parameters) {
        return Function(name, returnType, parameters, typeOfLocals, mainInstruction);
    }

public:
    static Function parse(ModuleContext& context, std::string name, Type returnType, std::vector<Type> parameters, ByteStream& stream) {
        FunctionParser parser(context, stream);
        parser.parse();
        return parser.getParsedFunction(name, returnType, parameters);
    }
};


#endif //WASMINT_FUNCTIONPARSER_H

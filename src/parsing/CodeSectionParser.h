//
// Created by teemperor on 19.06.15.
//

#ifndef INTERPRETER_SECTIONPARSER_H
#define INTERPRETER_SECTIONPARSER_H


#include <Function.h>
#include <ModuleContext.h>
#include "ByteStream.h"
#include "../Section.h"
#include "FunctionParser.h"

class CodeSectionParser {

    ByteStream& stream;

    std::vector<Function> functions;
    std::vector<FunctionSignature> signatures;

    ModuleContext& context;

    void parseFunctions() {
        uint32_t numberOfFunctions = stream.popULEB128();

        for (uint32_t i = 0; i < numberOfFunctions; i++) {
            std::string functionName = stream.readCString();

            Type* returnType = context.typeTable().getType(stream.popULEB128());

            uint32_t numberOfParameters = stream.popULEB128();
            std::vector<Type*> parameters;
            for (uint32_t j = 0; j < numberOfParameters; j++) {
                parameters.push_back(context.typeTable().getType(stream.popULEB128()));
            }

            uint32_t offset = stream.popULEB128();
            FunctionSignature signature = FunctionSignature(functionName, returnType, parameters);
            signatures.push_back(signature);
            context.functionTable().addFunctionSignature(signature);
        }

        for (int i = 0; i < numberOfFunctions; i++) {
            functions.push_back(FunctionParser::parse(context, signatures[i], stream));
        }
    }

protected:
    CodeSectionParser(ModuleContext& context, ByteStream& stream)
            : stream(stream), context(context) {

    }

    Section get(uint32_t offset) {
        return Section(offset, SectionType::CODE, functions);
    }

public:
    static Section parse(uint32_t offset, ModuleContext& context, ByteStream& stream) {
        CodeSectionParser parser(context, stream);
        parser.parseFunctions();
        return parser.get(offset);
    }
};


#endif //INTERPRETER_SECTIONPARSER_H

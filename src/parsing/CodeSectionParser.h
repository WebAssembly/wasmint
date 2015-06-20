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
    std::vector<std::string> functionNames;
    std::vector<Type> returnTypes;
    std::vector<uint32_t > offsets;

    ModuleContext& context;

    void parseFunctions() {
        uint32_t numberOfFunctions = stream.popLEB128();

        for (uint32_t i = 0; i < numberOfFunctions; i++) {
            std::string functionName = stream.readCString();
            functionNames.push_back(functionName);

            Type* returnType = context.typeTable().getType(stream.popLEB128());

            uint32_t numberOfParameters = stream.popLEB128();
            std::vector<Type*> parameters;
            for (uint32_t j = 0; j < numberOfParameters; j++) {
                parameters.push_back(context.typeTable().getType(stream.popLEB128()));
            }

            offsets.push_back(stream.popLEB128());
            context.functionTable().addFunctionSignature(FunctionSignature(functionName, returnType, parameters));
        }

        for (int i = 0; i < numberOfFunctions; i++) {
            functions.push_back(FunctionParser::parse(context, functionNames.at(i), Void::instance(), std::vector<Type*>(), stream));
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

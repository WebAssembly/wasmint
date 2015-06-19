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
            functionNames.push_back(stream.readCString());
            uint32_t returnTypeData = stream.popLEB128();

            uint32_t numberOfParameters = stream.popLEB128();
            for (uint32_t j = 0; j < numberOfParameters; j++) {
                stream.popLEB128();
            }

            offsets.push_back(stream.popLEB128());
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

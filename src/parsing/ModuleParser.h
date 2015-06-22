

#ifndef INTERPRETER_MODULEPARSER_H
#define INTERPRETER_MODULEPARSER_H


#include "../Module.h"
#include <map>
#include <OpcodeTable.h>
#include <GlobalTable.h>
#include "ByteStream.h"
#include "CodeSectionParser.h"
#include "OpcodeTableParser.h"
#include "TypeTableParser.h"
#include "FunctionTableParser.h"
#include "GlobalTableParser.h"

ExceptionMessage(NoSectionWithOffset)
ExceptionMessage(SectionTableNotOrdered)

class ModuleParser {

    ByteStream& stream;
    std::map<uint32_t, SectionType> sectionTypes;
    std::vector<std::string> requiredModules;

    std::vector<Section*> sections;

    ModuleContext context;

    SectionType getSectionTypeFromOffset(uint32_t offset) {
        auto result = sectionTypes.find(offset);

        if (result == sectionTypes.end()) {
            throw NoSectionWithOffset(std::to_string(offset));
        } else {
            return result->second;
        }
    }

protected:
    ModuleParser(ByteStream& stream) : stream(stream) {

    }

    void parseRequiredModules() {
        uint32_t numberOfModules = stream.popULEB128();
        for(uint32_t i = 0; i < numberOfModules; i++) {
            requiredModules.push_back(stream.readCString());
        }
    }

    void parseHeader() {
        parseRequiredModules();

        // Instruction table
        OpcodeTable opcodeTable = OpcodeTableParser::parse(stream);

        // Type table
        TypeTable typeTable = TypeTableParser::parse(stream);

        FunctionTable functionTable = FunctionTableParser::parse(stream);

        GlobalTable globalTable = GlobalTableParser::parse(stream, typeTable);

        // Put everything into the context
        context = ModuleContext(opcodeTable, typeTable, functionTable, globalTable);

        // Section header
        uint32_t numberOfSections = stream.popULEB128();

        uint32_t lastOffset = 0;

        for(uint32_t i = 0; i < numberOfSections; i++) {
            uint32_t typeData = stream.popULEB128();

            SectionType type;
            switch (typeData) {
                case 1:
                    type = SectionType::CODE;
                    break;
            }
            uint32_t offset = stream.popULEB128();

            sectionTypes[offset] = type;

            if (offset <= lastOffset)
                throw SectionTableNotOrdered(std::string("Offset of section ") + std::to_string(i) + " smaller than previous offset");
            lastOffset = offset;
        }
    }


    void parseSections() {
        SectionType sectionType = getSectionTypeFromOffset(stream.position());
        Section* section = CodeSectionParser::parse(stream.position(), context, stream);
        sections.push_back(section);
    }

    Module* getParsedModule() {
        return new Module(context, sections, requiredModules);
    }

public:
    static Module* parse(ByteStream& stream) {
        ModuleParser parser(stream);
        parser.parseHeader();
        parser.parseSections();
        return parser.getParsedModule();
    }
};


#endif //INTERPRETER_MODULEPARSER_H

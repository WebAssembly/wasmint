

#ifndef INTERPRETER_MODULEPARSER_H
#define INTERPRETER_MODULEPARSER_H


#include "../Module.h"
#include <map>
#include <OpcodeTable.h>
#include "ByteStream.h"
#include "CodeSectionParser.h"
#include "OpcodeTableParser.h"
#include "TypeTableParser.h"
#include "FunctionTableParser.h"

ExceptionMessage(NoSectionWithOffset)
ExceptionMessage(UnknownSectionType)
ExceptionMessage(SectionTableNotOrdered)

class ModuleParser {

    ByteStream& stream;
    std::map<uint32_t, SectionType> sectionTypes;

    std::vector<Section> sections;

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

    void parseHeader() {
        // Instruction table
        OpcodeTable opcodeTable = OpcodeTableParser::parse(stream);

        // Type table
        TypeTable typeTable = TypeTableParser::parse(stream);

        FunctionTable functionTable = FunctionTableParser::parse(stream);

        // Put everything into the context
        context = ModuleContext(opcodeTable, typeTable, functionTable);

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
                default:
                    throw UnknownSectionType(std::to_string(typeData));
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
        Section section = CodeSectionParser::parse(stream.position(), context, stream);
        sections.push_back(section);
    }

    Module* getParsedModule() {
        return new Module(context, sections);
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

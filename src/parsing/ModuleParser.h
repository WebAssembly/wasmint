//
// Created by teemperor on 19.06.15.
//

#ifndef INTERPRETER_MODULEPARSER_H
#define INTERPRETER_MODULEPARSER_H


#include "../Module.h"
#include <map>
#include <OpcodeTable.h>
#include "ByteStream.h"
#include "SectionParser.h"
#include "OpcodeTableParser.h"
#include "TypeTableParser.h"


class NoSectionWithOffset : public std::exception {};
class UnknownSectionType : public std::exception {};
class SectionTableNotOrdered : public std::exception {};

class ModuleParser {

    ByteStream& stream;
    std::map<uint32_t, SectionType> sectionTypes;

    std::vector<Section> sections;

    OpcodeTable opcodeTable;
    TypeTable typeTable;

    SectionType getSectionTypeFromOffset(uint32_t offset) {
        auto result = sectionTypes.find(offset);

        if (result == sectionTypes.end()) {
            throw NoSectionWithOffset();
        } else {
            return result->second;
        }
    }

protected:
    ModuleParser(ByteStream& stream) : stream(stream) {

    }

    void parseHeader() {
        // Instruction table
        opcodeTable = OpcodeTableParser::parse(stream);

        // Type table
        typeTable = TypeTableParser::parse(stream);


        // Section header
        uint32_t numberOfSections = stream.popLEB128();
        sections.resize(numberOfSections);

        uint32_t lastOffset = 0;

        for(uint32_t i = 0; i < numberOfSections; i++) {
            uint32_t typeData = stream.popLEB128();

            SectionType type;
            switch (typeData) {
                case 0:
                    type = SectionType::DATA;
                    break;
                case 1:
                    type = SectionType::CODE;
                    break;
                default:
                    throw UnknownSectionType();
            }
            uint32_t offset = stream.popLEB128();

            sectionTypes[offset] = type;

            if (offset <= lastOffset)
                throw SectionTableNotOrdered();
            lastOffset = offset;
        }
    }


    void parseSections() {
        SectionType sectionType = getSectionTypeFromOffset(stream.position());
        Section section = SectionParser::parse(stream);
    }

    Module* getParsedModule() {
        return new Module(opcodeTable, sections);
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

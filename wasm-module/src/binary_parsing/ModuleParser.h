/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

namespace wasm_module { namespace binary {

    ExceptionMessage(NoSectionWithOffset)

    ExceptionMessage(SectionTableNotOrdered)

    class ModuleParser {

        ByteStream &stream;
        std::map<uint32_t, SectionType> sectionTypes;
        std::vector<std::string> requiredModules;

        std::vector<Section *> sections;

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
        ModuleParser(ByteStream &stream) : stream(stream) {

        }

        void parseRequiredModules() {
            uint32_t numberOfModules = stream.popULEB128();
            for (uint32_t i = 0; i < numberOfModules; i++) {
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

            // Put everything into the context
            context = ModuleContext(opcodeTable, typeTable, functionTable);

            // Section header
            uint32_t numberOfSections = stream.popULEB128();

            uint32_t lastOffset = 0;

            for (uint32_t i = 0; i < numberOfSections; i++) {
                uint32_t typeData = stream.popULEB128();

                SectionType type;
                switch (typeData) {
                    case 1:
                        type = SectionType::CODE;
                        break;
                    default:
                        throw std::domain_error("Unknown section type");
                }
                uint32_t offset = stream.popULEB128();

                sectionTypes[offset] = type;

                if (offset <= lastOffset)
                    throw SectionTableNotOrdered(
                            std::string("Offset of section ") + std::to_string(i) + " smaller than previous offset");
                lastOffset = offset;
            }
        }


        void parseSections() {
            SectionType sectionType = getSectionTypeFromOffset(stream.position());
            Section *section = CodeSectionParser::parse(stream.position(), context, stream);
            sections.push_back(section);
        }

        Module *getParsedModule() {
            return new Module(context, sections, requiredModules);
        }

    public:
        static Module *parse(ByteStream &stream) {
            ModuleParser parser(stream);
            parser.parseHeader();
            parser.parseSections();
            return parser.getParsedModule();
        }
    };

}}

#endif //INTERPRETER_MODULEPARSER_H

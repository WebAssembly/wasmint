

#ifndef INTERPRETER_MODULE_H
#define INTERPRETER_MODULE_H

#include <string>
#include "parsing/ByteStream.h"
#include "Section.h"
#include "OpcodeTable.h"
#include "TypeTable.h"
#include "ModuleContext.h"
#include <vector>

class Module {

    std::vector<Section*> sections_;
    ModuleContext context_;
    std::vector<std::string> requiredModules_;

public:
    Module(ModuleContext& context, std::vector<Section*> sections,
           std::vector<std::string> requiredModules)
        : sections_(sections), context_(context), requiredModules_(requiredModules)
    {
    }

    virtual ~Module() {
        for(Section* section : sections_) {
            delete section;
        }
    }

    std::vector<Function*> functions() {
        std::vector<Function*> result;
        for(Section* section : sections_) {
            std::vector<Function*> sectionFunctions = section->functions();
            for(Function* function : sectionFunctions) {
                result.push_back(function);
            }
        }
        return result;
    }

    std::vector<Section*> sections() {
        return sections_;
    }

    OpcodeTable& opcodeTable() {
        return context_.opcodeTable();
    }

    TypeTable& typeTable() {
        return context_.typeTable();
    }

    std::vector<Global> globals() {
        return context_.globalTable().internalGlobals();
    }

    std::vector<std::string> requiredModules() {
        return requiredModules_;
    }

};


#endif //INTERPRETER_MODULE_H

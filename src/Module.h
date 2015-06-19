//
// Created by teemperor on 19.06.15.
//

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

    std::vector<Section> sections_;
    ModuleContext context_;


public:
    Module(ModuleContext& context, std::vector<Section>& sections)
            : sections_(sections), context_(context) {
    }

    std::vector<Section>& sections() {
        return sections_;
    }

    OpcodeTable& opcodeTable() {
        return context_.opcodeTable();
    }

    TypeTable& typeTable() {
        return context_.typeTable();
    }

};


#endif //INTERPRETER_MODULE_H

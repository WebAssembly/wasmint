//
// Created by teemperor on 19.06.15.
//

#ifndef INTERPRETER_MODULE_H
#define INTERPRETER_MODULE_H

#include <string>
#include "parsing/ByteStream.h"
#include "Section.h"
#include "OpcodeTable.h"
#include <vector>

class Module {

    std::vector<Section> sections_;
    OpcodeTable opcodeTable_;

public:
    Module(OpcodeTable& opcodeTable, std::vector<Section>& sections)
            : sections_(sections), opcodeTable_(opcodeTable) {
    }

    std::vector<Section>& sections() {
        return sections_;
    }

    OpcodeTable& opcodeTable() {
        return opcodeTable_;
    }

};


#endif //INTERPRETER_MODULE_H

//
// Created by teemperor on 19.06.15.
//

#ifndef INTERPRETER_SECTIONPARSER_H
#define INTERPRETER_SECTIONPARSER_H


#include "ByteStream.h"
#include "../Section.h"

class SectionParser {

    ByteStream& stream;

protected:
    SectionParser(ByteStream& stream) : stream(stream) {

    }

public:
    static Section parse(ByteStream& stream) {
        SectionParser instance(stream);
    }
};


#endif //INTERPRETER_SECTIONPARSER_H

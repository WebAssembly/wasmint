//
// Created by teemperor on 19.06.15.
//

#ifndef INTERPRETER_SECTION_H
#define INTERPRETER_SECTION_H


#include <assert.h>
#include <cstdint>

enum class SectionType {
    DATA = 0,
    CODE = 1,
    INVALID = 200
};


class Section {

    SectionType type_;
    uint32_t offset_;

public:
    Section(uint32_t offset = 0, SectionType type = SectionType::INVALID) : offset_(offset), type_(type) {

    }

    SectionType type() {
        return type_;
    }

    uint32_t offset() {
        return offset_;
    }
};


#endif //INTERPRETER_SECTION_H

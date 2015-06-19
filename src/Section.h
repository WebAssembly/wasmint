//
// Created by teemperor on 19.06.15.
//

#ifndef INTERPRETER_SECTION_H
#define INTERPRETER_SECTION_H


#include <assert.h>
#include <cstdint>
#include "Function.h"

enum class SectionType {
    DATA = 0,
    CODE = 1,
    INVALID = 200
};

class NoFunctionWithThatName : std::exception {};

class Section {

    SectionType type_;
    uint32_t offset_;
    std::vector<Function> functions_;

public:
    Section(uint32_t offset, SectionType type, std::vector<Function> functions)
            : offset_(offset), type_(type), functions_(functions) {

    }

    SectionType type() {
        return type_;
    }

    uint32_t offset() {
        return offset_;
    }

    Function& getFunction(std::string name) {
        for(Function& function : functions_) {
            if (function.name() == name) {
                return function;
            }
        }
        throw NoFunctionWithThatName();
    }
};


#endif //INTERPRETER_SECTION_H

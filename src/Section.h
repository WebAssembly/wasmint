

#ifndef INTERPRETER_SECTION_H
#define INTERPRETER_SECTION_H


#include <cstdint>
#include <vector>

#include "ExceptionWithMessage.h"

enum class SectionType {
    DATA = 0,
    CODE = 1,
    INVALID = 200
};

ExceptionMessage(NoFunctionWithThatName)

class Function;

class Section {

    SectionType type_;
    uint32_t offset_;
    std::vector<Function*> functions_;

public:
    Section(uint32_t offset, SectionType type, std::vector<Function*> functions)
            : offset_(offset), type_(type), functions_(functions) {
    }

    virtual ~Section();

    SectionType type() {
        return type_;
    }

    uint32_t offset() {
        return offset_;
    }

    std::vector<Function*> functions() {
        std::vector<Function*> result;
        for(Function* function : functions_) {
            result.push_back(function);
        }
        return result;
    }

    Function & getFunction(std::string name);
};


#endif //INTERPRETER_SECTION_H

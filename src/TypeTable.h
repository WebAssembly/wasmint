

#ifndef WASMINT_TYPETABLE_H
#define WASMINT_TYPETABLE_H

#include <string>
#include <cstdint>
#include <map>
#include "types/Type.h"
#include "ExceptionWithMessage.h"

ExceptionMessage(UnknownLocalTypeCode)

class TypeTable {


    std::map<uint32_t, std::string> typesByLocalTypeCode;

public:
    TypeTable() {
    }

    void addType(uint32_t localTypeCode, std::string name) {
        typesByLocalTypeCode[localTypeCode] = name;
    }

    Type * getType(uint32_t localTypeCode);

    std::string getTypeName(uint32_t localTypeCode) {
        auto result = typesByLocalTypeCode.find(localTypeCode);
        if (result == typesByLocalTypeCode.end()) {
            throw UnknownLocalTypeCode(std::to_string(localTypeCode));
        } else {
            return result->second;
        }
    }
};


#endif //WASMINT_TYPETABLE_H



#include "TypeTable.h"
#include <types/Void.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float64.h>
#include <types/Float32.h>

Type *TypeTable::getType(uint32_t localTypeCode) {
    std::string typeName = getTypeName(localTypeCode);
    if (typeName == "void") {
        return Void::instance();
    } else if (typeName == "int32") {
        return Int32::instance();
    } else if (typeName == "int364") {
        return Int64::instance();
    } else if (typeName == "float32") {
        return Float32::instance();
    } else if (typeName == "float364") {
        return Float64::instance();
    }
}
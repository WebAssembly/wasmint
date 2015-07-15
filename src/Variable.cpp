#include <types/Void.h>
#include "Variable.h"
#include "types/Type.h"

Variable::Variable(Type* type) : type_(type) {
    value_.resize(type->size());
}

Variable::Variable() : type_(Void::instance()) {
}

void Variable::setValue(std::vector<uint8_t> newData) {
    if (newData.size() != type_->size())
        throw InvalidDataSize();
    value_ = newData;
}
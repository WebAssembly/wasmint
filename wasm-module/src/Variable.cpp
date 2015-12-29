/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <types/Void.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>
#include "Variable.h"
#include "types/Type.h"
#include <cstring>


namespace wasm_module {

    Variable::Variable(const wasm_module::Type *type) {
        setType(type);
    }

    Variable::Variable() : type_(wasm_module::Void::instance()) {
    }

    void Variable::setValue(std::vector<uint8_t> newData) {
        if (newData.size() != type_->size())
            throw InvalidDataSize();
        std::memcpy(value_, newData.data(), newData.size());
    }

    uint32_t Variable::uint32() const {
        return Int32::getUnsignedValue(*this);
    }

    int32_t Variable::int32() const {
        return Int32::getValue(*this);
    }

    Variable::Variable(uint32_t value) {
        setType(Int32::instance());
        uint32(value);
    }

    Variable::Variable(int32_t value) {
        setType(Int32::instance());
        int32(value);
    }

    void Variable::setType(const wasm_module::Type* type) {
        type_ = type;
    }

    void Variable::uint32(uint32_t value) {
        Int32::setUnsignedValue(*this, value);
    }

    void Variable::int32(int32_t value) {
        Int32::setValue(*this, value);
    }

    Variable::Variable(int64_t value) {
        setType(Int64::instance());
        Int64::setValue(*this, value);
    }

    Variable::Variable(uint64_t value) {
        setType(Int64::instance());
        Int64::setUnsignedValue(*this, value);
    }

    Variable::Variable(float value) {
        setType(Float32::instance());
        Float32::setValue(*this, value);
    }

    Variable::Variable(double value) {
        setType(Float64::instance());
        Float64::setValue(*this, value);
    }

    Variable Variable::Void() {
        return Variable(Void::instance());
    }

    float Variable::float32() const {
        return Float32::getValue(*this);
    }

    double Variable::float64() const {
        return Float64::getValue(*this);
    }

    void Variable::float32(float value) {
        Float32::setValue(*this, value);
    }

    void Variable::float64(double value) {
        Float64::setValue(*this, value);
    }

    bool Variable::isVoid() const {
        return type_ == Void::instance();
    }

    std::vector<uint8_t> Variable::data() {
        std::vector<uint8_t> result;
        result.resize(type_->size());
        std::memcpy(result.data(), value_, type_->size());
        return result;
    }

    int64_t Variable::int64() const {
        return Int64::getValue(*this);
    }

    uint64_t Variable::uint64() const {
        return Int64::getUnsignedValue(*this);
    }

    std::string Variable::toString() const {
        if (type_ == Void::instance()) {
            return "void";
        }
        if (type_ == Int32::instance()) {
            return std::to_string(int32()) + " (" + std::to_string(uint32()) + "u)";
        }
        if (type_ == Int64::instance()) {
            return std::to_string(int64()) + " (" + std::to_string(uint64()) + "u)";
        }
        if (type_ == Float32::instance()) {
            return std::to_string(float32());
        }
        if (type_ == Float64::instance()) {
            return std::to_string(float64());
        }
        throw std::domain_error("Unknown variable type. Can't execute toString()");
    }

    bool Variable::operator==(const Variable& variable) const {
        if (type_ != variable.type_)
            return false;
        if (std::memcmp(value_, variable.value_, type().size()) != 0)
            return false;
        return true;
    }
}
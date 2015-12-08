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

#include "ByteOutputStream.h"
#include <types/Void.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>

void wasmint::ByteOutputStream::writeVariable(const wasm_module::Variable& variable) {
    if (&variable.type() == wasm_module::Void::instance())
        writeByte(0);
    else if (&variable.type() == wasm_module::Int32::instance())
        writeByte(1);
    else if (&variable.type() == wasm_module::Int64::instance())
        writeByte(2);
    else if (&variable.type() == wasm_module::Float32::instance())
        writeByte(3);
    else if (&variable.type() == wasm_module::Float64::instance())
        writeByte(4);
    else
        throw std::domain_error("Type " + variable.type().name() + " is not serializeable");

    uint8_t* data = (uint8_t*) variable.value();
    for (std::size_t i = 0; i < variable.type().size(); i++) {
        writeByte(*(data + i));
    }
}

void wasmint::ByteOutputStream::writeUInt32(uint32_t value) {
    outVector_->push_back((uint8_t) (value & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 8) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 16) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 24) & 0xFF));
}

void wasmint::ByteOutputStream::writeInt32(int32_t value) {
    writeUInt32((uint32_t) value);
}

void wasmint::ByteOutputStream::writeUInt64(uint64_t value) {
    outVector_->push_back((uint8_t) (value & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 8) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 16) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 24) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 32) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 40) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 48) & 0xFF));
    outVector_->push_back((uint8_t) ((value >> 56) & 0xFF));
}

void wasmint::ByteOutputStream::writeInt64(int64_t value) {
    writeUInt64((uint64_t) value);
}
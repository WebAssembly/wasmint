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


#ifndef WASMINT_BYTECODE_H
#define WASMINT_BYTECODE_H

#include <vector>
#include <cstdint>
#include "ByteOpcodes.h"
#include <interpreter/SafeAddition.h>
#include <stdexcept>
#include <cstring>
#include <iostream>

namespace wasmint {

    class ByteCode {

        std::vector<uint8_t> byteCode_;

    public:
        ByteCode() {
        }

        template<typename T>
        void get(T* target, std::size_t position) const {
            std::size_t size = sizeof(T);
            std::size_t unused;
            if (safeSizeTAddition(position, size, &unused)) {
                throw std::out_of_range("Can't call get on bytecode with length " + std::to_string(byteCode_.size())
                + " with pos " + std::to_string(position) + " and size " + std::to_string(size));
            }
            std::memcpy(target, byteCode_.data() + position, size);
        }

        template<typename T>
        T get(std::size_t position) const {
            T target;
            std::size_t size = sizeof(T);
            std::size_t unused;
            if (safeSizeTAddition(position, size, &unused)) {
                throw std::out_of_range("Can't call get on bytecode with length " + std::to_string(byteCode_.size())
                                        + " with pos " + std::to_string(position) + " and size " + std::to_string(size));
            }
            std::memcpy(&target, byteCode_.data() + position, size);
            return target;
        }

        template<typename T>
        void getUnsafe(T* target, std::size_t position) const {
            (*target) = *((T*) (byteCode_.data() + position));
        }

        void appendOpcode(ByteOpcode opcode) {
            append((uint16_t) opcode);
        }

        template<typename T>
        void append(T value) {
            byteCode_.resize(byteCode_.size() + sizeof value);
            memcpy(byteCode_.data() + (byteCode_.size() - sizeof(value)), &value, sizeof(value));
        }

        template<typename T>
        void write(std::size_t offset, T value) {
            if (offset + sizeof(value) > byteCode_.size()) {
                throw std::domain_error(std::to_string(offset) + " + " + std::to_string(sizeof(value)) + " <= " + std::to_string(byteCode_.size()) + " failed");
            }
            memcpy(byteCode_.data() + offset, &value, sizeof(value));
        }

        uint32_t size() const {
            return (uint32_t) byteCode_.size();
        }
    };
}



#endif //WASMINT_BYTECODE_H

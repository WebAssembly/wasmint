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

        std::size_t usedCodeSize_ = 0;
        std::vector<uint32_t> byteCode_;

        const uint8_t* data() const {
            return (uint8_t *) byteCode_.data();
        }

        uint8_t* data() {
            return (uint8_t *) byteCode_.data();
        }
        
    public:
        ByteCode() {
        }

        template<typename T>
        void get(T* target, std::size_t position) const {
            std::size_t size = sizeof(T);
            std::size_t unused;
            if (safeSizeTAddition(position, size, &unused)) {
                throw std::out_of_range("Can't call get on bytecode with length " + std::to_string(usedCodeSize_)
                + " with pos " + std::to_string(position) + " and size " + std::to_string(size));
            }
            std::memcpy(target, data() + position, size);
        }

        template<typename T>
        T get(std::size_t position) const {
            T target;
            std::size_t size = sizeof(T);
            std::size_t unused;
            if (safeSizeTAddition(position, size, &unused)) {
                throw std::out_of_range("Can't call get on bytecode with length " + std::to_string(usedCodeSize_)
                                        + " with pos " + std::to_string(position) + " and size " + std::to_string(size));
            }
            std::memcpy(&target, data() + position, size);
            return target;
        }

        template<typename T>
        void getUnsafe(T* target, std::size_t position) const {
            (*target) = *((T*) (data() + position));
        }

        void appendOpcode(ByteOpcode opcode) {
            append((uint32_t) opcode);
        }

        template<typename T>
        void append(T value) {
            usedCodeSize_ += sizeof value;
            if (usedCodeSize_ > byteCode_.size() * 4) {
                if (usedCodeSize_ % 4 == 0) {
                    byteCode_.resize(usedCodeSize_ / 4);
                } else {
                    // round up to the next word
                    byteCode_.resize((usedCodeSize_ / 4 + 1) * 4);
                }
            }
            memcpy(data() + (usedCodeSize_ - sizeof(value)), &value, sizeof(value));
        }

        template<typename T>
        void write(std::size_t offset, T value) {
            if (offset + sizeof(value) > usedCodeSize_) {
                throw std::domain_error(std::to_string(offset) + " + " + std::to_string(sizeof(value)) + " <= " + std::to_string(usedCodeSize_) + " failed");
            }
            memcpy(data() + offset, &value, sizeof(value));
        }

        uint32_t size() const {
            return (uint32_t) usedCodeSize_;
        }
    };
}



#endif //WASMINT_BYTECODE_H

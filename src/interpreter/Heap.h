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



#ifndef WASMINT_HEAP_H
#define WASMINT_HEAP_H

#include <cstdint>
#include <vector>
#include <ExceptionWithMessage.h>
#include <HeapData.h>
#include <string.h>

namespace wasmint {

    ExceptionMessage(OverFlowInHeapAccess)

    ExceptionMessage(OutOfBounds)

    class Heap {

        std::vector<uint8_t> data_;

        bool highestBitSet(uint32_t a) {
            return (a & (1u << 31)) != 0;
        }

        bool addition_is_safe(uint32_t a, uint32_t b) {
            return !highestBitSet(a) && !highestBitSet(b);
        }

    public:
        Heap() {
        }

        Heap(const wasm_module::HeapData& data) {
            data_.resize(data.startSize());
            std::fill(data_.begin(), data_.end(), 0);

            for (const wasm_module::HeapSegment& segment : data.segments()) {
                std::copy(segment.data().begin(), segment.data().end(), data_.begin() + segment.offset());
            }

        }

        void grow(uint32_t size) {
            std::size_t oldSize = data_.size();
            data_.resize(data_.size() + size);
            std::fill(data_.begin() + oldSize, data_.end(), 0);
        }

        std::vector<uint8_t> getBytes(uint32_t offset, uint32_t size) {
            if (!addition_is_safe(offset, size))
                throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                                                    + " + size " + std::to_string(size));

            if (offset + size > data_.size()) {
                throw OutOfBounds(std::string("Offset ") + std::to_string(offset)
                                                           + " + size " + std::to_string(size));
            }

            std::vector<uint8_t> result;
            result.resize(size);

            for (uint32_t i = offset; i < offset + size; i++) {
                result[i - offset] = data_[i];
            }
            return result;
        }

        void setBytes(uint32_t offset, std::vector<uint8_t> bytes) {
            if (!addition_is_safe(offset, (uint32_t) bytes.size()))
                throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                                                    + " + size " + std::to_string(bytes.size()));

            if (offset + bytes.size() > data_.size()) {
                throw OutOfBounds(std::string("Offset ") + std::to_string(offset)
                                                           + " + size " + std::to_string(bytes.size()));
            }

            for (uint32_t i = offset; i < offset + bytes.size(); i++) {
                data_[i] = bytes[i - offset];
            }
        }

        std::string getString(uint32_t offset) {
            uint32_t endOffset = 0;
            bool foundOffset = false;
            for(uint32_t i = offset; i < data_.size(); i++) {
                if (data_[i] == '\0') {
                    endOffset = i;
                    foundOffset = true;
                }
            }

            std::string result;
            result.reserve(endOffset - offset);

            for(uint32_t i = offset; i < endOffset; i++) {
                result[i - offset] = data_[i];
            }
            return result;
        }

        std::size_t size() const {
            return data_.size();
        }
    };

}

#endif //WASMINT_HEAP_H

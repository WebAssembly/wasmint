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
#include <serialization/Serializeable.h>
#include <serialization/ByteInputStream.h>
#include "HeapPatch.h"

namespace wasmint {

    ExceptionMessage(OverFlowInHeapAccess)
    ExceptionMessage(OutOfBounds)
    ExceptionMessage(CantChangeHeapSize)

    class HeapPatch;

    class Heap : public Serializeable {

        const static std::size_t maxSize_ = 1073741824;
        std::vector<uint8_t> data_;

        bool highestBitSet(std::size_t a) {
            std::size_t i = 1;
            return (a & (i << ((sizeof a) * 8 - 1))) != 0;
        }

        bool isAdditionSafe(std::size_t a, std::size_t b) {
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

        std::size_t maxSize() const {
            return maxSize_;
        }

        void applyPatch(const HeapPatch& patch);

        void setState(ByteInputStream& stream);

        uint8_t getByte(std::size_t pos) {
            return data_.at(pos);
        }

        uint8_t setByte(std::size_t pos, uint8_t value) {
            return data_.at(pos) = value;
        }

        void grow(std::size_t size) {

            std::size_t oldSize = data_.size();

            if (!isAdditionSafe(oldSize, size)) {
                throw CantChangeHeapSize("Can't grow heap with size " + std::to_string(oldSize) + " by " + std::to_string(size) + " because ");
            }
            std::size_t newSize = oldSize + size;

            if (newSize > maxSize_)
                throw CantChangeHeapSize("New heap size of " + std::to_string(newSize) + " is bigger than the allowed max size of " + std::to_string(maxSize_));

            data_.resize(newSize);
            std::fill(data_.begin() + oldSize, data_.end(), 0);
        }

        void grow(std::size_t size, HeapPatch& patch) {
            HeapPatch::createMemoryShrinked(patch, size);
            grow(size);
        }

        void shrink(std::size_t size) {

            if (size > data_.size())
                throw CantChangeHeapSize("Can't shrink memory of size " + std::to_string(data_.size()) + " by " + std::to_string(size));
            data_.resize(data_.size() - size);
        }

        void shrink(std::size_t size, HeapPatch& patch) {
            std::vector<uint8_t> oldBytes;
            oldBytes.resize(size);
            memcpy(oldBytes.data(), data_.data() + (data_.size() - size), size);
            HeapPatch::createMemoryGrow(patch, data_.size() - size, oldBytes);

            shrink(size);
        }

        void setBytes(std::size_t offset, const std::vector<uint8_t>& bytes) {
            if (!isAdditionSafe(offset, bytes.size()))
                throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                           + " + size " + std::to_string(bytes.size()));

            if (offset + bytes.size() > data_.size()) {
                throw OutOfBounds(std::string("Offset ") + std::to_string(offset)
                                  + " + size " + std::to_string(bytes.size()));
            }

            for (std::size_t i = offset; i < offset + bytes.size(); i++) {
                data_[i] = bytes[i - offset];
            }
        }

        void setBytes(uint32_t offset, const std::vector<uint8_t>& bytes, HeapPatch& patch) {

            std::vector<uint8_t> oldBytes;
            oldBytes.resize(bytes.size());
            memcpy(oldBytes.data(), data_.data() + offset, bytes.size());
            HeapPatch::createMemoryChanged(patch, offset, oldBytes);

            setBytes(offset, bytes);
        }

        std::vector<uint8_t> getBytes(std::size_t offset, uint32_t size) {
            if (!isAdditionSafe(offset, size))
                throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                                                    + " + size " + std::to_string(size));

            if (offset + size > data_.size()) {
                throw OutOfBounds(std::string("Offset ") + std::to_string(offset)
                                                           + " + size " + std::to_string(size));
            }

            std::vector<uint8_t> result;
            result.resize(size);

            for (std::size_t i = offset; i < offset + size; i++) {
                result[i - offset] = data_[i];
            }
            return result;
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

        virtual void serialize(ByteOutputStream& stream) const override;

        bool operator==(const Heap& other) const;

        bool operator!=(const Heap& other) const {
            return !this->operator==(other);
        }
    };

}

#endif //WASMINT_HEAP_H

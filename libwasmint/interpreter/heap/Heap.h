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
#include <serialization/ByteInputStream.h>
#include <serialization/ByteOutputStream.h>
#include "../SafeAddition.h"
#include <cstring>

namespace wasmint {

    ExceptionMessage(OverFlowInHeapAccess)
    ExceptionMessage(OutOfBounds)
    ExceptionMessage(CantChangeHeapSize)

    class HeapPatch;

    class Heap {

        const static std::size_t maxSize_ = 1073741824;
        std::vector<uint8_t> data_;

        const static std::size_t pageSize_ = 1024;

    public:
        Heap() {
        }

        Heap(std::size_t size) {
            data_.resize(size, 0);
        }

        Heap(const wasm_module::HeapData& data) {
            data_.resize(data.startSize());
            std::fill(data_.begin(), data_.end(), 0);

            for (const wasm_module::HeapSegment& segment : data.segments()) {
                std::copy(segment.data().begin(), segment.data().end(), data_.begin() + segment.offset());
            }

        }

        std::size_t pageSize() const {
            return pageSize_;
        }

        std::size_t maxSize() const {
            return maxSize_;
        }

        void setState(ByteInputStream& stream);

        uint8_t getByte(std::size_t pos) {
            return data_.at(pos);
        }

        uint8_t setByte(std::size_t pos, uint8_t value) {
            return data_.at(pos) = value;
        }

        void grow(std::size_t size) {
            std::size_t oldSize = data_.size();
            std::size_t newSize;

            if (safeSizeTAddition(oldSize, size, &newSize)) {
                throw CantChangeHeapSize("Can't grow heap with size " + std::to_string(oldSize) + " by " + std::to_string(size) + " as this would cause a integer overflow");
            }

            if (newSize > maxSize_)
                throw CantChangeHeapSize("New heap size of " + std::to_string(newSize) + " is bigger than the allowed max size of " + std::to_string(maxSize_));

            data_.resize(newSize);
            std::fill(data_.begin() + oldSize, data_.end(), 0);
        }

        void grow(std::size_t size, HeapPatch& patch);

        void shrink(std::size_t size) {
            if (size > data_.size())
                throw CantChangeHeapSize("Can't shrink memory of size " + std::to_string(data_.size()) + " by " + std::to_string(size));
            data_.resize(data_.size() - size);
        }

        void resize(std::size_t size) {
            if (size > maxSize_) {
                throw CantChangeHeapSize("Can't set memory size to " + std::to_string(size) + " because it is bigger than the max size " + std::to_string(maxSize_));
            }
            data_.resize(size);
        }

        void setBytes(std::size_t offset, const std::vector<uint8_t>& bytes) {
            std::size_t end;

            if (safeSizeTAddition(offset, bytes.size(), &end)) {
                throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                           + " + size " + std::to_string(bytes.size()));
            }

            if (end > data_.size()) {
                throw OutOfBounds(std::string("Offset ") + std::to_string(offset)
                                  + " + size " + std::to_string(bytes.size()));
            }

            for (std::size_t i = offset; i < offset + bytes.size(); i++) {
                data_[i] = bytes[i - offset];
            }
        }

        template<typename T>
        bool setStaticOffset(std::size_t staticOffset, std::size_t offset, T value) {
            std::size_t end;

            if (safeSizeTAddition(offset, sizeof(T), &end)) {
                return false;
            }
            if (safeSizeTAddition(staticOffset, end, &end)) {
                return false;
            }

            if (end > data_.size()) {
                return false;
            }

            std::memcpy(data_.data() + offset + staticOffset, &value, sizeof(T));

            return true;
        }

        template<typename T>
        bool set(std::size_t offset, T value) {
            std::size_t end;

            if (safeSizeTAddition(offset, sizeof(T), &end)) {
                return false;
            }

            if (end > data_.size()) {
                return false;
            }

            std::memcpy(data_.data() + offset, &value, sizeof(T));

            return true;
        }

        void setBytes(uint32_t offset, const std::vector<uint8_t>& bytes, HeapPatch& patch);

        template<typename T>
        bool get(std::size_t offset, T* value) {
            std::size_t end;

            if (safeSizeTAddition(offset, sizeof(T), &end)) {
                return false;
            }

            if (end > data_.size()) {
                return false;
            }

            std::memcpy(value, data_.data() + offset, sizeof(T));

            return true;
        }

        template<typename T>
        bool getStaticOffset(std::size_t offset, std::size_t staticOffset, T* value) {
            std::size_t end;

            if (safeSizeTAddition(offset, sizeof(T), &end)) {
                return false;
            }
            if (safeSizeTAddition(staticOffset, end, &end)) {
                return false;
            }

            if (end > data_.size()) {
                return false;
            }

            std::memcpy(value, data_.data() + offset + staticOffset, sizeof(T));

            return true;
        }

        std::vector<uint8_t> getBytes(std::size_t offset, std::size_t size) {

            std::size_t end;

            if (safeSizeTAddition(offset, size, &end)) {
                throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                           + " + size " + std::to_string(size));
            }

            if (end > data_.size()) {
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

        virtual void serialize(ByteOutputStream& stream) const;

        bool operator==(const Heap& other) const;

        bool operator!=(const Heap& other) const {
            return !this->operator==(other);
        }
    };

}

#endif //WASMINT_HEAP_H

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


#ifndef WASMINT_HEALMULTIPATCHPART_H
#define WASMINT_HEALMULTIPATCHPART_H

#include <stdexcept>
#include <vector>
#include <cstring>

namespace wasmint {

    class HeapMultiPatchPart {
        std::vector<uint8_t> bytes_;
        std::size_t position_;

        void mergeInternal(const HeapMultiPatchPart& other) {
            std::size_t newEnd = std::max(end(), other.end());
            std::size_t newStart = std::min(position_, other.position_);
            std::size_t oldSize = bytes_.size();
            bytes_.resize(newEnd - newStart);
            if (position_ <= other.position_) {
                std::size_t otherOffset = other.position_ - position_;
                std::memcpy(bytes_.data() + otherOffset, other.bytes_.data(), other.bytes_.size());
            } else { // position_ > other.position_
                std::size_t offset =  position_ - other.position_;
                std::memmove(bytes_.data() + offset, bytes_.data(), oldSize);
                std::memcpy(bytes_.data(), other.bytes_.data(), other.bytes_.size());
                position_ = newStart;
            }
        }

    public:
        HeapMultiPatchPart(std::size_t position, std::vector<uint8_t> bytes) : position_(position), bytes_(bytes) {
            if (bytes_.empty())
                throw std::domain_error("Can't create HeapMultiPatchPart with empty content");
        }

        bool merge(const HeapMultiPatchPart& other) {
            bool result = canMerge(other);
            if (result) {
                mergeInternal(other);
            }
            return result;
        }

        bool canMerge(const HeapMultiPatchPart& other) {
            return overlaps(other) || adjacent(other);
        }

        bool adjacent(const HeapMultiPatchPart& other) {
            return position_ == other.end() || other.position_ == this->end();
        }

        bool overlaps(const HeapMultiPatchPart& other) {
            return position_ < other.end() && other.position_ < end();
        }

        bool operator<(const HeapMultiPatchPart& other) const {
            return position_ < other.position_;
        }

        /**
         * Exclusive end of this part.
         */
        std::size_t end() const {
            return position_ + bytes_.size();
        }

        std::size_t start() const {
            return position_;
        }

        bool shrink(std::size_t newGlobalEnd) {
            if (end() > newGlobalEnd && position_ < newGlobalEnd) {
                bytes_.resize(newGlobalEnd - position_);
                return true;
            }
            return false;
        }

        const std::vector<uint8_t> bytes() const {
            return bytes_;
        }

        bool operator==(const HeapMultiPatchPart& other) const {
            if (position_ != other.position_)
                return false;
            if (bytes_.size() != other.bytes_.size())
                return false;
            return memcmp(bytes_.data(), other.bytes_.data(), bytes_.size()) == 0;
        }
    };

}

#endif //WASMINT_HEALMULTIPATCHPART_H

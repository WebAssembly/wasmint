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


#ifndef WASMINT_HEAPPATCH_H
#define WASMINT_HEAPPATCH_H

#include <cstdint>
#include <vector>
#include "Heap.h"

namespace wasmint {

    class HeapPatch {

        std::size_t position_ = 0;
        std::vector<uint8_t> data_;
        std::size_t memorySizeDiff_ = 0;
        bool memorySizeGrows_ = true;

    public:
        HeapPatch() {
        }

        static void createMemoryChanged(HeapPatch& patch, std::size_t position, const std::vector<uint8_t> &data) {
            patch.data_ = data;
            patch.position_ = position;
            patch.memorySizeGrows_ = true;
            patch.memorySizeDiff_ = 0;
        }

        static void createMemoryShrinked(HeapPatch& patch, std::size_t diff) {
            patch.data_.clear();
            patch.position_ = 0;
            patch.memorySizeDiff_ = diff;
            patch.memorySizeGrows_ = false;
        }

        void apply(Heap& heap) {
            if (onlyChangesMemory()) {
                heap.setBytes(position(), data());
            } else {
                if (memorySizeGrows()) {
                    throw std::domain_error("Can't apply a HeapPatch that grows memory");
                } else {
                    heap.shrink(memorySizeDiff());
                }
            }
        }

        std::size_t position() const {
            return position_;
        }

        const std::vector<uint8_t> &data() const {
            return data_;
        }

        bool onlyChangesMemory() const {
            return memorySizeDiff_ == 0;
        }

        std::size_t memorySizeDiff() const {
            return memorySizeDiff_;
        }

        bool memorySizeGrows() const {
            return memorySizeGrows_;
        }

    };
}

#endif //WASMINT_HEAPPATCH_H

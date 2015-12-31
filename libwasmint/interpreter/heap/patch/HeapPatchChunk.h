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


#ifndef WASMINT_HEAPPATCHCHUNK_H
#define WASMINT_HEAPPATCHCHUNK_H

#include <cstdint>
#include <vector>
#include <interpreter/heap/Interval.h>

namespace wasmint {
    class HeapPatchChunk : public Interval {
        std::vector<uint8_t> data_;

    public:
        HeapPatchChunk() {
        }
        HeapPatchChunk(Interval interval) : Interval(interval) {
            data_.resize(interval.size());
        }

        void setByte(std::size_t pos, uint8_t value) {
            data_[pos - start()] = value;
        }

        const std::vector<uint8_t> data() const {
            return data_;
        }

    };
}



#endif //WASMINT_HEAPPATCHCHUNK_H

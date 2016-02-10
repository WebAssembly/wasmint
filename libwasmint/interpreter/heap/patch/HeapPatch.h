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

#include <stdexcept>
#include <vector>
#include <cstring>
#include <set>
#include <map>

#include <interpreter/heap/Heap.h>
#include <interpreter/heap/Interval.h>
#include <interpreter/heap/patch/HeapPatchChunk.h>

namespace wasmint {

    class HeapPatch {

        std::size_t heapSize_;

        std::map<std::size_t, HeapPatchChunk> chunks_;

    public:
        static const std::size_t chunkSize = 1024;

        HeapPatch() {
        }
        HeapPatch(Heap& heap) : heapSize_(heap.size()) {
            static_assert(sizeof(void*) == sizeof(std::size_t), "std::size_t is not able to hold a pointer value which "
                "is necessary for the judy array implementation");
        }

        void applyPatch(Heap& heap) const {
            heap.resize(heapSize_);
            for (auto& pair : chunks_) {
                const HeapPatchChunk& chunk = pair.second;
                heap.setBytes(chunk.start(), chunk.data());
            }
        }

        void preHeapChanged(const Heap& heap, Interval changedInterval) {
            if (changedInterval.start() >= heapSize_)
                return;

            std::size_t chunkIndex = changedInterval.start() / chunkSize;
            changedInterval = Interval::withEnd(chunkIndex * chunkSize, (chunkIndex + 1) * chunkSize);
            if (changedInterval.end() > heapSize_) {
                changedInterval.end(heapSize_);
            }

            if (chunks_.find(chunkIndex) == chunks_.end()) {
                HeapPatchChunk& chunk = chunks_[chunkIndex] = HeapPatchChunk(changedInterval);
                for (std::size_t i = changedInterval.start(); i < changedInterval.end(); i++) {
                    chunk.setByte(i, heap.getByte(i));
                }
            }
        }

        std::set<std::size_t> modifiedChunks() const {
            std::set<std::size_t> result;
            for (auto& pair : chunks_) {
                result.insert(pair.first);
            }
            return result;
        }
    };
}

#endif //WASMINT_HEAPPATCH_H

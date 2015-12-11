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


#ifndef WASMINT_HEAPMULTIPATCH_H
#define WASMINT_HEAPMULTIPATCH_H

#include <stdexcept>
#include <vector>
#include <cstring>
#include <set>

#include "HeapMultiPatchPart.h"
#include "Heap.h"

namespace wasmint {

    class HeapMultiPatch {

        std::set<HeapMultiPatchPart> parts_;
        std::size_t heapSize_;

    public:
        HeapMultiPatch(std::size_t heapSize) : heapSize_(heapSize) {
        }

        void setBytes(std::size_t position, const std::vector<uint8_t>& bytes) {
            if (position >= heapSize_)
                return;

            HeapMultiPatchPart newPatch(position, bytes);

            newPatch.shrink(heapSize_);

            auto iter = parts_.begin();
            while (iter != parts_.end())
            {
                bool merge = newPatch.merge(*iter);

                if (merge)
                {
                    iter = parts_.erase(iter);

                    if (newPatch.start() > iter->start())
                        break;
                }
                else
                {
                    iter++;
                }
            }
            parts_.insert(newPatch);
        }

        // TODO this getter is bad design...
        const std::set<HeapMultiPatchPart>& parts() {
            return parts_;
        }

        void applyPatch(Heap& heap) {
            heap.resize(heapSize_);
            for (const HeapMultiPatchPart& part : parts_) {
                heap.setBytes(part.start(), part.bytes());
            }
        }

        void rescueBytes(Heap& heap, std::size_t position, std::size_t length) {
            setBytes(position, heap.getBytes(position, length));
        }
    };
}

#endif //WASMINT_HEAPMULTIPATCH_H

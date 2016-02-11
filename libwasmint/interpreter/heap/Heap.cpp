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



#include "Heap.h"

namespace wasmint {

    void Heap::serialize(ByteOutputStream& stream) const {
        stream.writeBytes(data_);
    }

    void Heap::setState(ByteInputStream& stream) {
        data_ = stream.getBytes();
    }


    bool Heap::operator==(const Heap& other) const {
        if (other.size() != size())
            return false;
        if (size() == 0)
            return true;
        return memcmp(other.data_.data(), data_.data(), size()) == 0;
    }
}
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


#ifndef WASMINT_BYTEOUTPUTSTREAM_H
#define WASMINT_BYTEOUTPUTSTREAM_H

#include <cstdint>
#include <vector>
#include <Variable.h>

namespace wasmint {

    class ByteOutputStream {

        std::vector<uint8_t>* outVector_ = nullptr;

    public:
        ByteOutputStream() {
        }

        ByteOutputStream(std::vector<uint8_t>& outVector) : outVector_(&outVector) {

        }

        void writeByte(uint8_t byte) {
            outVector_->push_back(byte);
        }

        void writeUInt32(uint32_t value);

        void writeInt32(int32_t value);

        void writeUInt64(uint64_t value);

        void writeInt64(int64_t value);

        void writeVariable(const wasm_module::Variable& variable);

    };
}

#endif //WASMINT_BYTEOUTPUTSTREAM_H

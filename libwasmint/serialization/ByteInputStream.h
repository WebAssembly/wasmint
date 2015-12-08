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


#ifndef WASMINT_BYTEINPUTSTREAM_H
#define WASMINT_BYTEINPUTSTREAM_H

#include <cstdint>
#include <vector>
#include <Variable.h>
#include <instructions/InstructionAddress.h>

namespace wasmint {

    class ByteInputStream {

        std::vector<uint8_t>* inputVector_ = nullptr;

        std::size_t position_ = 0;

    public:
        ByteInputStream(std::vector<uint8_t>& inputVector) : inputVector_(&inputVector) {

        }

        uint8_t getByte() {
            uint8_t result = *(inputVector_->data() + position_);
            position_++;
            return result;
        }

        uint32_t getUInt32() {
            uint32_t result = 0;
            result |= ((uint32_t) getByte()) << 0;
            result |= ((uint32_t) getByte()) << 8;
            result |= ((uint32_t) getByte()) << 16;
            result |= ((uint32_t) getByte()) << 24;
            return result;
        }

        int32_t getInt32() {
            return (int32_t) getUInt32();
        }

        uint64_t getUInt64() {
            uint64_t result = 0;
            result |= ((uint64_t) getByte()) << 0;
            result |= ((uint64_t) getByte()) << 8;
            result |= ((uint64_t) getByte()) << 16;
            result |= ((uint64_t) getByte()) << 24;
            result |= ((uint64_t) getByte()) << 32;
            result |= ((uint64_t) getByte()) << 40;
            result |= ((uint64_t) getByte()) << 48;
            result |= ((uint64_t) getByte()) << 56;
            return result;
        }

        int64_t getInt64() {
            return (int64_t) getUInt64();
        }

        wasm_module::Variable getVariable();

        std::string getString() {
            std::string result;
            uint64_t size = getUInt64();
            result.reserve(size);
            for (std::size_t i = 0; i < size; i++) {
                result.push_back((char) getByte());
            }
            return result;
        }

        std::vector<uint8_t> getBytes() {
            std::vector<uint8_t> result;
            uint64_t size = getUInt64();
            result.reserve(size);
            for (std::size_t i = 0; i < size; i++) {
                result.push_back(getByte());
            }
            return result;
        }

        bool getBool() {
            return getByte() != 0;
        }

        wasm_module::InstructionAddress getInstructionAddress() {
            std::string module = getString();
            std::string functionName = getString();

            std::vector<std::size_t> indizes;
            uint64_t indizesSize = getUInt64();
            for (uint64_t i = 0; i < indizesSize; i++) {
                indizes.push_back(getUInt64());
            }
            return wasm_module::InstructionAddress(module, functionName, indizes);
        }
    };

}



#endif //WASMINT_BYTEINPUTSTREAM_H

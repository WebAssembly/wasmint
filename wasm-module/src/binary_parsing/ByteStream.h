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

#ifndef INTERPRETER_BYTESTREAM_H
#define INTERPRETER_BYTESTREAM_H


#include <cstdint>
#include <deque>
#include <string>
#include <sstream>
#include <vector>

namespace wasm_module { namespace binary {

    class LEB128PayloadBiggerThan32Bit : public std::exception {
    };

    class EndOfStreamReached : public std::exception {
    };

    class ByteStream {

        std::deque<uint8_t> bytes_;
        uint32_t position_ = 0;

    public:
        ByteStream(std::deque<uint8_t> bytes) : bytes_(bytes) {
        }

        ByteStream(std::vector<uint8_t> bytes) {
            bytes_.resize(bytes.size());
            for (uint32_t i = 0; i < bytes.size(); i++) {
                bytes_[i] = bytes[i];
            }
        }

        ByteStream(const ByteStream &copy); // Don't implement to prevent copying

        uint8_t popChar() {
            position_++;
            uint8_t result = peekChar();
            bytes_.pop_front();

            return result;
        }

        std::string readCString() {
            std::string result = "";
            while (peekChar() != 0) {
                result.push_back((unsigned char) popChar());
            }
            // remove the \0 at the end
            popChar();
            return result;
        }

        uint8_t peekChar() {
            if (bytes_.empty())
                throw EndOfStreamReached();
            return bytes_.front();
        }


        uint32_t popULEB128() {
            uint32_t result = 0;
            uint8_t shift = 0;
            for (int i = 0; i < 5; i++) {
                uint8_t byte = popChar();

                // check if first bit is set
                if (byte >= 128u) {
                    // if we currently check the 4th byte, the number has to be smaller than 128
                    // or the payload would be bigger thatn 32 bit (which is restricted by the specification)
                    if (i == 4) {
                        throw LEB128PayloadBiggerThan32Bit();
                    }
                }

                // OR the new byte without the MSB into the result integer
                result |= (byte & 0x7F) << shift;

                // if the MSB is not set, we are done here
                if (byte < 128u) {
                    break;
                }
                shift += 7;
            }
            return result;
        }

        uint32_t position() {
            return position_;
        }

    };

}}

#endif //INTERPRETER_BYTESTREAM_H

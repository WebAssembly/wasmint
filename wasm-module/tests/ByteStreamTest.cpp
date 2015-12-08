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

#include <cstdint>
#include <binary_parsing/ByteStream.h>
#include <Module.h>
#include <binary_parsing/ModuleParser.h>
#include <cassert>
#include <types/Int32.h>

using namespace wasm_module;

int main() {
    std::deque<uint8_t> data = {
            0x10, // 16u
            0xE5, 0x8E, 0x26, // 624485u
            0x10, // 16s
            0xE5, 0x8E, 0x26, // 624485s
            0x9b, 0xf1, 0x59 // -624485s
    };

    binary::ByteStream stream(data);

    uint32_t leb = stream.popULEB128();
    assert (leb == 16);

    leb = stream.popULEB128();
    assert (leb == 624485);

    int32_t sleb = Int32::getFromStream(stream);
    assert(sleb == 16);

    sleb = Int32::getFromStream(stream);
    assert(sleb == 624485);

    sleb = Int32::getFromStream(stream);
    assert(sleb == -624485);
}
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

#ifndef WASMINT_OPCODETABLE_H
#define WASMINT_OPCODETABLE_H

#include <cstdint>
#include <string>
#include <map>
#include "ExceptionWithMessage.h"


namespace wasm_module {

    ExceptionMessage(UnknownLocalOpcode)

    class OpcodeTable {

        std::map<uint32_t, std::string> instructionsByLocalOpcode;

    public:
        OpcodeTable() {
        }

        void addInstruction(uint32_t localOpcode, std::string name) {
            instructionsByLocalOpcode[localOpcode] = name;
        }

        std::string getInstruction(uint32_t localOpcode) {
            auto result = instructionsByLocalOpcode.find(localOpcode);
            if (result == instructionsByLocalOpcode.end()) {
                throw UnknownLocalOpcode(std::to_string(localOpcode));
            } else {
                return result->second;
            }
        }
    };

}

#endif //WASMINT_OPCODETABLE_H

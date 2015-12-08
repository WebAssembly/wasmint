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


#ifndef WASMINT_INSTRUCTIONADDRESS_H
#define WASMINT_INSTRUCTIONADDRESS_H

#include <string>
#include <vector>

namespace wasm_module {
    class InstructionAddress {

        std::string moduleName_, functionName_;
        std::vector<std::size_t> childrenIndizes_;

    public:
        InstructionAddress(const std::string& moduleName, const std::string& functionName, const std::vector<std::size_t>& childrenIndizes);

        const std::string& moduleName() const {
            return moduleName_;
        }

        const std::string& functionName() const {
            return functionName_;
        }

        const std::vector<std::size_t> childrenIndized() const {
            return childrenIndizes_;
        }

    };
}

#endif //WASMINT_INSTRUCTIONADDRESS_H

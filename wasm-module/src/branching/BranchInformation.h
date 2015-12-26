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


#ifndef WASMINT_BRANCHINFORMATION_H
#define WASMINT_BRANCHINFORMATION_H

#include <cstdint>
#include <types/Void.h>
#include <ExceptionWithMessage.h>

namespace wasm_module {
    ExceptionMessage(CantFindBranchTarget);

    class Instruction;

    class BranchInformation {

        // the specific label index at the target instruction
        uint32_t labelIndex_ = 0;
        uint32_t label_ = 0;
        const Instruction* target_ = nullptr;
        const Type* valueType_ = Void::instance();
        bool terminatesOnlyCurrentInstruction = true;

    public:
        BranchInformation() {
        }

        BranchInformation(uint32_t labelIndex, const Instruction* target, uint32_t label, const Type* valueType = Void::instance());

        uint32_t labelIndex() const {
            return labelIndex_;
        }

        uint32_t label() const {
            return label_;
        }

        const Instruction* target() const {
            return target_;
        }

        const Type* valueType() const {
            return valueType_;
        }

        static BranchInformation getBranchInformation(const Instruction& start, const std::string& labelName, const Type* valueType);

        static BranchInformation getBranchInformation(const Instruction &start, uint32_t label,
                                                      const Type *valueType);
    };
}

#endif //WASMINT_BRANCHINFORMATION_H

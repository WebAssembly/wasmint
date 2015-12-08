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


#include "BranchTypeValidator.h"
#include <instructions/Instruction.h>

namespace wasm_module {

    const Type* wasm_module::BranchTypeValidator::checkBranchType(const Instruction& instruction, uint32_t onlyLabelIndex) {

        const Type* type = nullptr;

        instruction.foreachChild([&](const Instruction* child) {
            const BranchInformation* branchInformation = child->branchInformation();

            if (branchInformation) {
                if (branchInformation->target() == &instruction && branchInformation->labelIndex() == onlyLabelIndex) {
                    if (type) {
                        if (type != branchInformation->valueType()) {
                            throw IncompatibleBranchTypes(type->name() + " and " + branchInformation->valueType()->name());
                        }
                    }

                    type = branchInformation->valueType();
                }
            }
        });

        return type;
    }
}
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


#include "BranchInformation.h"
#include <instructions/Instruction.h>

namespace wasm_module {

wasm_module::BranchInformation::BranchInformation(uint32_t labelIndex, const Instruction* target, uint32_t label, const Type* valueType)
    : labelIndex_(labelIndex), target_(target), label_(label), valueType_(valueType) {

}

BranchInformation wasm_module::BranchInformation::getBranchInformation(const Instruction& start, const std::string& labelName, const Type* valueType) {
    uint32_t labelIndex = 0;
    uint32_t label = 0;
    const Instruction* target = nullptr;

    if (!labelName.empty()) {
        bool foundTarget = false;
        start.foreachParent([&](const Instruction* instruction) {
            if (instruction->hasLabelName(labelName)) {
                target = instruction;
                label += instruction->labelIndex(labelName);
                labelIndex = instruction->labelIndex(labelName);
                foundTarget = true;
                return false;
            } else {
                label += instruction->labelCount();
                return true;
            }
        });
        if (!foundTarget) {
            throw CantFindBranchTarget("Can't find branch target: " + labelName);
        }
    } else {
        throw std::domain_error("labelName can't be empty");
    }
    return BranchInformation(labelIndex, target, label, valueType);
}

BranchInformation wasm_module::BranchInformation::getBranchInformation(const Instruction &start, uint32_t label,
                                                                       const Type *valueType) {
    uint32_t labelIndex = 0;
    const Instruction* target = nullptr;
    bool foundTarget = false;

    uint32_t labelCounter = label;
    start.foreachParent([&](const Instruction* instruction) {
        if (instruction->labelCount() < labelCounter) {
            labelCounter -= instruction->labelCount();
            return true;
        } else {
            target = instruction;
            labelIndex = labelCounter;
            foundTarget = true;
            return false;
        }
    });
    if (!foundTarget) {
        throw CantFindBranchTarget("Can't find branch target: " + std::to_string(label));
    }
    return BranchInformation(labelIndex, target, label, valueType);
}

    bool BranchInformation::targetsStart() const {
        return target()->id() == InstructionId::Loop && labelIndex() == 0;
    }
}
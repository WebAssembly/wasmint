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


#include <types/Void.h>
#include "Instruction.h"
#include "UnreachableValidator.h"
#include "InstructionAddress.h"

namespace wasm_module {

    void Instruction::triggerSecondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {
        for(Instruction* instruction : children_) {
            instruction->triggerSecondStepEvaluate(context, functionContext);
        }
        if (typeCheckChildren()) {
            if (children_.size() != childrenTypes().size()) {
                throw IncompatibleNumberOfChildren(name() + " got " + std::to_string(children_.size()) + " children, but expected " +  std::to_string(childrenTypes().size()));
            }

            for (std::size_t i = 0; i < children_.size(); i++) {
                // skip type check if the given child instruction will certainly terminate this program
                if (UnreachableValidator::willExecuteUnreachable(children_[i])) {
                    continue;
                }
                if (!Type::typeCompatible(childrenTypes()[i], children_[i]->returnType())) {
                    throw IncompatibleChildReturnType(name() + " expected " + childrenTypes()[i]->name() + " but got " + children_[i]->returnType()->name());
                }
            }
        }
        secondStepEvaluate(context, functionContext);
    }

    InstructionAddress Instruction::getAddress(const std::string& moduleName, const std::string& functionName) const {
        std::vector<size_t> childrenIndizes;

        const Instruction* childInstruction = this;
        foreachParent([&](const Instruction* instruction) {
            childrenIndizes.push_back(instruction->getChildIndex(childInstruction));
            childInstruction = instruction;
            return true;
        });

        return InstructionAddress(moduleName, functionName, childrenIndizes);
    }

    std::size_t Instruction::getChildIndex(const Instruction* instruction) const {
        for (size_t i = 0; i < children().size(); i++) {
            if (children()[i] == instruction)
                return i;
        }
        throw std::domain_error("getChildIndex(instr) ");
    }
}
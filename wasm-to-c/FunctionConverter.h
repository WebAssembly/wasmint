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


#ifndef WASMINT_FUNCTIONCONVERTER_H
#define WASMINT_FUNCTIONCONVERTER_H

#include <string>
#include <instructions/Instruction.h>
#include <map>

class FunctionConverter {

    std::map<const wasm_module::Instruction*, std::string> subexprVariableNames_;
    std::map<const wasm_module::Instruction*, std::string> subexprLabelNames_;
    const wasm_module::Function* function_ = nullptr;

public:
    void addInstruction(const wasm_module::Instruction* instruction) {
        subexprVariableNames_[instruction] = "sub" + std::to_string(subexprVariableNames_.size());
    }

    void tryCreateLabel(const wasm_module::Instruction* instruction) {
        switch(instruction->id()) {
            case InstructionId::Label:
            case InstructionId::Block:
            case InstructionId::Loop:
            case InstructionId::TableSwitch:
                subexprLabelNames_[instruction] = std::string("label") + std::to_string(subexprLabelNames_.size());
                break;
            default:
                break;
        }
    }

    std::string getLabel(const wasm_module::Instruction* instruction) {
        auto iter = subexprLabelNames_.find(instruction);

        if (iter != subexprLabelNames_.end()) {
            return iter->second;
        } else {
            throw std::domain_error("No known label for instruction " + instruction->toSExprString());
        }
    }

    std::string operator()(const wasm_module::Instruction* instruction, const char* cast) {
        return "((" + std::string(cast) + ") " + subexprVariableNames_.at(instruction) + ")";
    }

    std::string operator()(const wasm_module::Instruction* instruction) {
        return subexprVariableNames_.at(instruction);
    }

    void setFunction(const wasm_module::Function* function) {
        function_ = function;
    }

    const wasm_module::Function* function() const {
        return function_;
    }

};


#endif //WASMINT_FUNCTIONCONVERTER_H

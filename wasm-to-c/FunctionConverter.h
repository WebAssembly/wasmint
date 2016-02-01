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
    std::map<const wasm_module::Instruction*, std::string> floatLiteralVariables_;
    const wasm_module::Function* function_ = nullptr;

public:
    void addInstruction(const wasm_module::Instruction* instruction) {
        std::size_t index = subexprVariableNames_.size();
        subexprVariableNames_[instruction] = "sub" + std::to_string(index);
        if (instruction->id() == InstructionId::F32Const || instruction->id() == InstructionId::F64Const) {
            floatLiteralVariables_[instruction] = "floatValue" + std::to_string(index);
        }
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

    const std::map<const wasm_module::Instruction*, std::string> floatLiteralVariables() {
        return floatLiteralVariables_;
    }

    const std::string& getFloatLiteralVariable(const wasm_module::Instruction* instruction) {
        auto iter = floatLiteralVariables_.find(instruction);

        if (iter != floatLiteralVariables_.end()) {
            return iter->second;
        } else {
            throw std::domain_error("No known float literal value for instruction " + instruction->toSExprString());
        }
    }

    const std::string& getLabel(const wasm_module::Instruction* instruction) {
        auto iter = subexprLabelNames_.find(instruction);

        if (iter != subexprLabelNames_.end()) {
            return iter->second;
        } else {
            throw std::domain_error("No known label for instruction " + instruction->toSExprString());
        }
    }

    std::string operator()(const wasm_module::Instruction* instruction, const char* cast) {
        if (instruction->id() == InstructionId::Unreachable)
            return "0"; // this could should never be reached. TODO: abort program if it is reached anyways
        return "((" + std::string(cast) + ") " + subexprVariableNames_.at(instruction) + ")";
    }

    std::string operator()(const wasm_module::Instruction* instruction) {
        if (instruction->id() == InstructionId::Unreachable)
            return "0"; // this could should never be reached. TODO: abort program if it is reached anyways
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

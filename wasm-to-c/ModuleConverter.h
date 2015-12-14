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


#ifndef WASMINT_MODULECONVERTER_H
#define WASMINT_MODULECONVERTER_H

#include <string>
#include <Module.h>
#include "FunctionConverter.h"

class ModuleConverter {

    std::stringstream cSource_;
    wasm_module::Module* module_;
    FunctionConverter functionConverter;

    void indent(std::size_t indentation) {
        for (std::size_t i = 0; i < indentation; i++) {
            cSource_ << " ";
        }
    }

    std::string functionName(const wasm_module::FunctionSignature* function);

    std::string variableName(const wasm_module::Function* function, std::size_t variableIndex);

    std::string toCType(const wasm_module::Type* type);

    void appendFunctionDeclaration(const wasm_module::Function* function);

    void appendFunctionDefinition(const wasm_module::Function* function);

    void serializeInstruction(const wasm_module::Instruction& instruction, std::size_t indentation);

    void convertFunction(const wasm_module::Function* function);

    void convert();

    void appendTrap(const wasm_module::Instruction& instruction, std::string reason, std::size_t indentation);

public:
    ModuleConverter(wasm_module::Module* module) :module_(module) {
        convert();
    }

    std::string cSource() {
        return cSource_.str();
    }

};


#endif //WASMINT_MODULECONVERTER_H

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

#ifndef WASMINT_MODULECONTEXT_H
#define WASMINT_MODULECONTEXT_H


#include "TypeTable.h"
#include "OpcodeTable.h"
#include "FunctionTable.h"
#include "FunctionTypeTable.h"

namespace wasm_module {

    ExceptionMessage(NoImportWithName)
    ExceptionMessage(AlreadyImportWithName)

    class ModuleContext {

        OpcodeTable opcodeTable_;
        TypeTable typeTable_;
        FunctionTable mainFunctionTable_;
        FunctionTable importedFunctionTable_;
        FunctionTable indirectCallTable_;
        FunctionTypeTable functionTypeTable_;

        std::string name_;

    public:
        ModuleContext() {
            name_ = "UnknownModule" + std::to_string(rand());
        }

        ModuleContext(OpcodeTable &opcodeTable, TypeTable &typeTable, FunctionTable &functionTable)
                : opcodeTable_(opcodeTable), typeTable_(typeTable), mainFunctionTable_(functionTable) {
            name_ = "UnknownModule" + std::to_string(rand());

        }

        const std::string& name() const {
            return name_;
        }

        const std::string& name(const std::string& newName) {
            name_ = newName;
            return name();
        }

        OpcodeTable &opcodeTable() {
            return opcodeTable_;
        }

        TypeTable &typeTable() {
            return typeTable_;
        }

        FunctionTable& mainFunctionTable() {
            return mainFunctionTable_;
        }

        FunctionTable& importedFunctionTable() {
            return importedFunctionTable_;
        }

        FunctionTable& indirectCallTable() {
            return indirectCallTable_;
        }

        const FunctionTable& indirectCallTable() const {
            return indirectCallTable_;
        }

        FunctionTypeTable& functionTypeTable() {
            return functionTypeTable_;
        }

    };

}

#endif //WASMINT_MODULECONTEXT_H

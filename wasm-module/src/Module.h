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

#ifndef INTERPRETER_MODULE_H
#define INTERPRETER_MODULE_H

#include <string>
#include "binary_parsing/ByteStream.h"
#include "Section.h"
#include "OpcodeTable.h"
#include "TypeTable.h"
#include "ModuleContext.h"
#include "FunctionContext.h"
#include "Function.h"
#include "HeapData.h"
#include "FunctionTypeTable.h"
#include <vector>
#include <functional>

namespace wasm_module {

    ExceptionMessage(NoFunctionWithName)

    class Module {

        std::vector<Section *> sections_;
        ModuleContext context_;
        std::vector<std::string> requiredModules_;
        std::vector<Function*> functions_;
        std::vector<Function*> functionsToDelete_;


        HeapData heapData_;

    public:
        Module(ModuleContext &context, std::vector<Section *> sections,
                       std::vector<std::string> requiredModules);
        Module() {
        }

        virtual ~Module() {
            for (Section* section : sections_) {
                delete section;
            }
            for (Function* function : functionsToDelete_) {
                delete function;
            }
        }

        const HeapData& heapData() const {
            return heapData_;
        }

        void heapData(const HeapData& data) {
            heapData_ = data;
        }

        const std::vector<Function *>& functions() {
            return functions_;
        }

        void addFunction(Function* function, bool takeMemoryOwnership = false) {
            function->module(this);
            functions_.push_back(function);
            if (takeMemoryOwnership)
                functionsToDelete_.push_back(function);
        }

        const Function* function(const std::string& functionName) const {
            for (const Function* function : functions_) {
                if (function->name() == functionName) {
                    return function;
                }
            }
            throw std::domain_error("Module " + functionName + " has no function with name " + name());
        }

        FunctionTable& mainFunctionTable() {
            return context().mainFunctionTable();
        }

        FunctionTable& importedFunctionTable() {
            return context().importedFunctionTable();
        }

        FunctionTypeTable& functionTypeTable() {
            return context().functionTypeTable();
        }

        ModuleContext& context() {
            return context_;
        }

        const ModuleContext& context() const {
            return context_;
        }

        void addFunction(std::string functionName, const Type* returnType, std::vector<const Type*> parameterTypes, std::function<Variable(std::vector<Variable>)> givenFunction);

        void addVariadicFunction(std::string functionName, const Type *returnType,
                                 std::function<Variable(std::vector<Variable>)> givenFunction);

        std::vector<Section *> sections() {
            return sections_;
        }

        const Function& getFunction(const std::string& functionName) const {
            for (Function* function : functions_) {
                if (function->name() == functionName) {
                    return *function;
                }
            }
            throw NoFunctionWithName(functionName + " in module " + name());
        }

        const std::string& name() const {
            return context_.name();
        }

        void name(const std::string& name) {
            context_.name(name);
        }

        OpcodeTable& opcodeTable() {
            return context_.opcodeTable();
        }

        TypeTable& typeTable() {
            return context_.typeTable();
        }

        std::vector<std::string> requiredModules() {
            return requiredModules_;
        }

    };

}

#endif //INTERPRETER_MODULE_H

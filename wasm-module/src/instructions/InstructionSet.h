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

#ifndef WASMINT_INSTRUCTIONSET_H
#define WASMINT_INSTRUCTIONSET_H

#include <string>
#include <FunctionContext.h>
#include <ExceptionWithMessage.h>
#include <ModuleContext.h>
#include <sexpr_parsing/SExpr.h>
#include "Instruction.h"
#include <set>

namespace wasm_module {

    ExceptionMessage(UnknownInstructionName)

    class InstructionSet {

        static Instruction* getInstruction(std::string name, ModuleContext &context,
                                                   FunctionContext &functionContext);

    public:
        static Instruction* getInstruction(std::string name, binary::ByteStream &stream, ModuleContext &context,
                                           FunctionContext &functionContext);


        static Instruction* getInstruction(std::string name, const sexpr::SExpr& expr, ModuleContext &context,
                                                   FunctionContext &functionContext, std::set<std::size_t>& subExprsToIgnore);
    };
}

#endif //WASMINT_INSTRUCTIONSET_H

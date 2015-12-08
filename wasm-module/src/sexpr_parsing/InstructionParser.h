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

#ifndef WASMINT_INSTRUCTIONPARSER_H
#define WASMINT_INSTRUCTIONPARSER_H

#include <instructions/Instruction.h>
#include <ModuleContext.h>
#include <FunctionContext.h>
#include <instructions/InstructionSet.h>
#include <instructions/Instructions.h>
#include "SExpr.h"

namespace wasm_module { namespace sexpr {

    ExceptionMessage(EmptyExpression)

    class InstructionParser {

        Instruction* instruction = nullptr;

        ModuleContext& moduleContext_;
        FunctionContext& functionContext_;

        Instruction* parse(const SExpr& expr) {
            if (expr.children().size() != 0) {
                if (expr[0].hasValue()) {
                    std::set<std::size_t> subExprToIgnore;
                    Instruction* result = InstructionSet::getInstruction(expr[0].value(), expr, moduleContext_, functionContext_, subExprToIgnore);

                    std::vector<Instruction*> children;

                    std::size_t index = 0;
                    for (const SExpr& child : expr.children()) {
                        if (subExprToIgnore.find(index) == subExprToIgnore.end()) {
                            if (!child.hasValue())
                                children.push_back(parse(child));
                        }
                        index++;
                    }

                    result->children(children);

                    return result;
                } else {
                    Instruction* result = new Block((uint32_t) expr.children().size());

                    std::vector<Instruction*> children;

                    for (const SExpr& child : expr.children()) {
                        if (!child.hasValue())
                            children.push_back(parse(child));
                    }

                    result->children(children);

                    return result;
                }
            } else {
                return new Nop();
            }
        }

        InstructionParser(const SExpr& expr, ModuleContext& moduleContext, FunctionContext& functionContext)
                : moduleContext_(moduleContext), functionContext_(functionContext) {
            instruction = parse(expr);
        }

    public:
        static Instruction* parse(const SExpr& expr, ModuleContext& moduleContext, FunctionContext& functionContext) {
            InstructionParser parser(expr, moduleContext, functionContext);
            return parser.instruction;
        }

    };
}}


#endif //WASMINT_INSTRUCTIONPARSER_H

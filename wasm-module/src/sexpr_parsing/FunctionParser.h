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


#ifndef WASMINT_FUNCTIONPARSER_H
#define WASMINT_FUNCTIONPARSER_H

#include <Function.h>
#include <ExceptionWithMessage.h>
#include "SExpr.h"
#include "Types.h"
#include "InstructionParser.h"
#include <unordered_map>
#include <FunctionType.h>

namespace wasm_module { namespace sexpr {

    ExceptionMessage(UnexpectedTokenInFunction);
    ExceptionMessage(MissingFunctionName);
    ExceptionMessage(MultipleFunctionName);
    ExceptionMessage(MalformedLocalStatement);
    ExceptionMessage(MalformedParamStatement);
    ExceptionMessage(MalformedResultStatement);
    ExceptionMessage(MalformedTypeStatement);
    ExceptionMessage(MultipleReturnTypes);
    ExceptionMessage(VariableDeclarationAfterFunctionBody);
    ExceptionMessage(MultipleFunctionTypesDefined);
    ExceptionMessage(FunctionSignatureDoesntMatchType);

    class FunctionParser {

        ModuleContext& context_;

        std::vector<const Type*> locals;
        std::vector<const Type*> parameters;

        std::unordered_map<std::string, uint32_t> namesToIndex_;

        bool hasOwnSignature = false;
        bool hasFunctionTypeDefined = false;
        FunctionType functionType_;

        FunctionContext functionContext_;

        Function* function_;

        const Type* returnType = Void::instance();

        std::string functionName_;

        Instruction* instruction_ = nullptr;

        bool hasDollarPrefix(std::string value) {
            if (value.empty())
                return false;
            return value.at(0) == '$';
        }

        void addVariable(std::string variableName, std::string typeName) {
            const Type* type = Types::getByName(typeName);

            namesToIndex_[variableName] = parameters.size() + locals.size();
            locals.push_back(type);
        }

        void parseLocal(const SExpr& local) {
            if (local.children().size() == 3 && hasDollarPrefix(local[1].value())) {
                std::string variableName = local[1].value();
                std::string typeName = local[2].value();

                addVariable(variableName, typeName);
            } else {
                for (std::size_t i = 1; i < local.children().size(); i++) {
                    addVariable(std::to_string(parameters.size() + locals.size()), local[i].value());
                }
            }
        }

        void addParameter(std::string variableName, std::string typeName) {
            const Type* type = Types::getByName(typeName);

            namesToIndex_[variableName] = parameters.size() + locals.size();
            parameters.push_back(type);
        }

        void parseParam(const SExpr& param) {
            hasOwnSignature = true;
            if (param.children().size() == 3 && hasDollarPrefix(param[1].value())) {
                std::string variableName = param[1].value();
                std::string typeName = param[2].value();

                addParameter(variableName, typeName);
            } else {
                for (std::size_t i = 1; i < param.children().size(); i++) {
                    addParameter(std::to_string(parameters.size() + locals.size()), param[i].value());
                }
            }
        }

        void parseResult(const SExpr& result) {
            hasOwnSignature = true;
            if (result.children().size() == 2) {
                std::string resultTypeName = result[1].value();
                const Type* newReturnType = Types::getByName(resultTypeName);

                if (returnType != Void::instance()) {
                    throw MultipleReturnTypes(std::string("Return type was already defines as ") + returnType->name()
                                              + std::string(", can't assign new return type ")
                                              + newReturnType->name());
                }
                returnType = newReturnType;
            } else {
                throw MalformedResultStatement(result.toString());
            }
        }

        void parseInstructions(const sexpr::SExpr& expr, const std::vector<std::size_t>& instructionExprs) {
            if (instructionExprs.size() == 1) {
                instruction_ = InstructionParser::parse(expr[instructionExprs.front()], context_, functionContext_);
            } else {

                instruction_ = new Block((uint32_t) instructionExprs.size());
                std::vector<Instruction*> children;
                for (std::size_t i : instructionExprs) {
                    const sexpr::SExpr& subExpr = expr[i];
                    Instruction* newInstruction = InstructionParser::parse(subExpr, context_, functionContext_);
                    children.push_back(newInstruction);
                }
                instruction_->children(children);
            }
        }

        void parseType(const SExpr& result) {
            if (hasFunctionTypeDefined) {
                throw MultipleFunctionTypesDefined(result.toString());
            }
            if (result.children().size() == 2) {
                std::string typeId = result[1].value();
                if (Utils::hasDollarPrefix(typeId)) {
                    functionType_ = context_.functionTypeTable().getType(typeId);
                } else {
                    std::size_t typeIndex = Utils::strToSizeT(typeId);
                    functionType_ = context_.functionTypeTable().getType(typeIndex);
                }
                hasFunctionTypeDefined = true;
            } else {
                throw MalformedResultStatement(result.toString());
            }
        }

        FunctionParser(const SExpr& funcExpr, ModuleContext& context) : context_(context) {
            // FIXME this is obviously not a perfect solution

            std::vector<std::size_t> instructionExprs;

            for(unsigned i = 1; i < funcExpr.children().size(); i++) {
                const SExpr& expr = funcExpr[i];

                if (expr.hasValue()) {
                    continue;
                }

                if (expr.children().size() == 0 || !expr[0].hasValue()) {
                    instructionExprs.push_back(i);
                    continue;
                }

                const std::string& typeName = expr[0].value();

                if (typeName == "param") {
                    parseParam(expr);
                } else if (typeName == "local") {
                    parseLocal(expr);
                } else if (typeName == "result") {
                    parseResult(expr);
                } else if (typeName == "type") {
                    parseType(expr);
                } else if (typeName == "export") {
                    if (!functionName_.empty())
                        throw MultipleFunctionName(funcExpr.toString());
                    functionName_ = expr[1].value();
                } else {
                    instructionExprs.push_back(i);
                }
            }

            if (functionName_.empty())
                functionName_ = std::to_string(context.mainFunctionTable().size());

            if (hasFunctionTypeDefined && !hasOwnSignature) {
                returnType = functionType_.returnType();
                parameters = functionType_.parameters();
            }

            functionContext_ = FunctionContext(context_.name(), functionName_, returnType, parameters, locals);
            functionContext_.setVariableNameToIndexMap(namesToIndex_);

            if (hasFunctionTypeDefined) {
                functionContext_.index(functionType_.index());

                if (!functionType_.compatibleWith(functionContext_)) {
                    throw FunctionSignatureDoesntMatchType("Expected " + functionType_.toString() + " signature in this function: " + funcExpr.toString());
                }
            }

            parseInstructions(funcExpr, instructionExprs);
            function_ = new Function(functionContext_, instruction_);
        }

        Function& getParsedFunction() {
            return *function_;
        }


    public:
        static Function& parse(const SExpr& funcExpr, ModuleContext& context) {
            FunctionParser parser(funcExpr, context);
            return parser.getParsedFunction();
        }
    };

}}


#endif //WASMINT_FUNCTIONPARSER_H

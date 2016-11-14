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

#include <iostream>
#include <limits>
#include "ModuleParser.h"
#include "FunctionParser.h"
#include "SExprParser.h"


namespace wasm_module { namespace sexpr {
    ModuleParser::ModuleParser(const SExpr& moduleExpr, const std::string& nameHint) {
        module_->context().name(nameHint);
        if (moduleExpr[0].value() != "module") {
            throw std::domain_error("First child of a module expression needs to be \"module\"");
        }

        for(unsigned i = 1; i < moduleExpr.children().size(); i++) {
            const SExpr& expr = moduleExpr[i];
            const std::string& typeName = expr[0].value();
            if (typeName == "import") {
                parseImport(expr);
            } else if (typeName == "memory") {
                parseMemory(expr);
            } else if (typeName == "table") {
                // we will handle indirect call tables later
            } else if (typeName == "type") {
                parseFunctionType(expr);
            } else if (typeName == "export") {
                // will handle later
            } else if (typeName == "func") {
                Function* function = &FunctionParser::parse(expr, module_->context());
                module_->context().mainFunctionTable().addFunctionSignature(*function, function->name());
                module_->addFunction(function, true);
                // FIXME
                module_->addExport(function->name(), function);
            } else {
                throw UnknownModuleChild(typeName);
            }
        }

        for(unsigned i = 1; i < moduleExpr.children().size(); i++) {
            const SExpr& expr = moduleExpr[i];
            const std::string& typeName = expr[0].value();
            if (typeName == "table") {
                parseIndirectCallTable(expr);
            }
            if (typeName == "export") {
                parseExport(expr);
            }
        }

        for (Function* function : module_->functions()) {
            function->mainInstruction()->triggerSecondStepEvaluate(module_->context(), *function);
            // Check that the function body returns the correct type
            if (!Type::typeCompatible(function->returnType(), function->mainInstruction()->returnType())) {
                throw TypeMismatch("type mismatch");
            }
        }
    }

    void ModuleParser::parseImport(const SExpr& importExpr) {
        if (importExpr.children().size() < 4) {
            throw MalformedImportStatement(importExpr.toString());
        } else {
            if (importExpr[0] != "import") {
                throw MalformedImportStatement(importExpr.toString());
            }

            std::string importName = importExpr[1].value();
            std::string moduleName = importExpr[2].value();
            std::string functionName = importExpr[3].value();

            const Type* returnType = Void::instance();

            std::vector<const Type*> parameters;

            bool hasOwnSignature = false;
            bool hasFunctionType = false;
            FunctionType functionType;


            for (uint32_t i = 4; i < importExpr.children().size(); i++) {
                const SExpr& subExpr = importExpr[i];

                if (subExpr[0].value() == "param") {
                    hasOwnSignature = true;
                    for (std::size_t j = 1; j < subExpr.children().size(); j++)
                        parameters.push_back(Types::getByName(subExpr[j].value()));
                } else if (subExpr[0].value() == "result") {
                    hasOwnSignature = true;
                    if (returnType != Void::instance()) {
                        throw MultipleReturnTypesInImport(importExpr.toString());
                    }

                    returnType = Types::getByName(subExpr[1].value());
                } else if (subExpr[0].value() == "type") {
                    if (hasFunctionType) {
                        throw std::domain_error("Function declaration has multiple function types associated: " + importExpr.toString());
                    }
                    hasFunctionType = true;
                    std::string typeValue = subExpr[1].value();

                    if (Utils::hasDollarPrefix(typeValue)) {
                        functionType = module_->context().functionTypeTable().getType(typeValue);
                    } else {
                        functionType = module_->context().functionTypeTable().getType(Utils::strToSizeT(typeValue));
                    }

                    returnType = functionType.returnType();
                    parameters = functionType.parameters();
                } else {
                    throw UnknownImportExpressionChild(subExpr.toString());
                }
            }

            if (hasOwnSignature && hasFunctionType) {
                if (!functionType.compatibleWith(FunctionType(returnType, parameters))) {
                    throw std::domain_error("Function declaration signature doesn't match given function type: " + importExpr.toString());
                }
            }

            if (hasFunctionType) {
                module_->importedFunctionTable().addFunctionSignature(FunctionSignature(moduleName, functionName, functionType.returnType(), functionType.parameters()), importName);
            } else {
                module_->importedFunctionTable().addFunctionSignature(FunctionSignature(moduleName, functionName, returnType, parameters), importName);
            }
        }
    }

    void ModuleParser::parseMemory(const SExpr&memoryExpr) {
        if (memoryExpr.children().size() >= 2) {
            uint32_t startMem = (uint32_t) std::atoll(memoryExpr[1].value().c_str());
            uint32_t maxMem = 1024;
            bool hasMaxValue = false;

            if (memoryExpr.children().size() >= 3) {
                hasMaxValue = memoryExpr[2].hasValue();
            }

            if (hasMaxValue) {
                maxMem = (uint32_t) std::atoll(memoryExpr[2].value().c_str());
            }
            if (maxMem < startMem) {
                throw MalformedMemoryStatement("Max size smaller than start size " + memoryExpr.toString());
            }
            HeapData data(startMem, maxMem);

            for(uint32_t childNum = hasMaxValue ? 3 : 2; childNum < memoryExpr.children().size(); childNum++) {
                const SExpr& segmentExpr = memoryExpr[childNum];
                if (segmentExpr.children().size() >= 3) {
                    uint32_t offset = (uint32_t) std::atoll(segmentExpr[1].value().c_str());
                    // TODO char to uint8_t is maybe compiler specific
                    std::vector<uint8_t> segmentData = decodeEscapedString(segmentExpr[2].value());

                    HeapSegment segment(offset, segmentData);
                    data.addNextSegment(segment);
                } else {
                    throw MalformedMemoryStatement("Not enough children for a valid segment statement (needs at least 3): " + segmentExpr.toString());
                }
            }

            module_->heapData(data);
        } else {
            throw MalformedMemoryStatement("Not enough children for a valid memory statement (needs at least 3): " + memoryExpr.toString());
        }
    }

    void ModuleParser::parseFunctionType(const SExpr& functionTypeExpr) {
        if (functionTypeExpr.children().size() == 1 || functionTypeExpr.children().size() > 3) {
            throw std::domain_error("Malformed function type expr (wrong number of child expressions): " + functionTypeExpr.toString());
        }

        std::string alias;
        const Type* returnType = Void::instance();
        std::vector<const Type*> parameters;

        sexpr::SExpr funcExpr;
        if (functionTypeExpr[1].hasChildren()) {
            funcExpr = functionTypeExpr[1];
        } else {
            alias = functionTypeExpr[1].value();
            funcExpr = functionTypeExpr[2];
        }

        if (funcExpr[0].value() != "func") {
            throw std::domain_error("Malformed func statement: " + funcExpr.toString());
        }

        bool foundResultStatement = false;

        for (std::size_t i = 1; i < funcExpr.children().size(); i++) {
            const SExpr& expr = funcExpr[i];

            if (expr.children().size() == 0) {
                throw std::domain_error("Malformed func statement: " + funcExpr.toString());
            }

            if (expr[0].value() == "param") {
                if (expr.children().size() > 1) {
                    for (std::size_t j = 1; j < expr.children().size(); j++) {
                        parameters.push_back(Types::getByName(expr[j].value()));
                    }
                } else if (expr.children().size() == 1) {
                    // void param: (param)
                }
            } else if (expr[0].value() == "result") {
                if (foundResultStatement) {
                    throw std::domain_error("Multiple result statements in func statement: " + funcExpr.toString());
                }
                foundResultStatement = true;

                if (expr.children().size() > 2) {
                    throw std::domain_error("Malformed result statement: " + funcExpr.toString());
                } else if (expr.children().size() == 1) {
                    // void result: (result)
                } else {
                    returnType = Types::getByName(expr[1].value());
                }
            } else {
                throw std::domain_error("Unknown expression in func statement: " + funcExpr.toString());
            }
        }

        if (alias.empty())
            module_->functionTypeTable().addFunctionType(FunctionType(returnType, parameters));
        else
            module_->functionTypeTable().addFunctionType(FunctionType(returnType, parameters), alias);

    }

    void ModuleParser::parseIndirectCallTable(const SExpr &functionTypeTableExpr) {
        for (std::size_t i = 1; i < functionTypeTableExpr.children().size(); i++) {
            std::string aliasName = functionTypeTableExpr[i].value();
            if (Utils::hasDollarPrefix(aliasName)) {
                FunctionSignature signature = module_->mainFunctionTable().getFunctionSignature(aliasName);
                module_->context().indirectCallTable().addFunctionSignature(signature);
            } else {
                throw std::domain_error("Malformed function type alias: " + aliasName);
            }
        }
    }

    std::vector<uint8_t> ModuleParser::decodeEscapedString(const std::string& str) const {
        std::vector<uint8_t> result;
        result.reserve(str.size());
        for (std::size_t i = 0; i < str.size(); i++) {
            char c = str[i];
            if (c == '\\') {
                if (i + 2 < str.size()) {
                    result.push_back(decodeHexEscape(str[i + 1], str[i + 2]));
                    i += 2;
                } else {
                    throw InvalidHexEncoding("Unfinished hexadecimal byte encoding at end of string " + str);
                }
            } else {
                result.push_back((uint8_t) c);
            }
        }
        return result;
    }

    uint8_t ModuleParser::decodeHexEscape(char c1, char c2) const {
        uint8_t result = hexToUint8(c1);
        result <<= 4;
        result |= hexToUint8(c2);
        return result;

    }

    uint8_t ModuleParser::hexToUint8(char character) const {
        if (character >= 'A' && character <= 'F') {
            return (uint8_t) ((character - 'A') + 10);
        }
        if (character >= 'a' && character <= 'f') {
            return (uint8_t) ((character - 'a') + 10);
        }
        if (character >= '0' && character <= '9') {
            return (uint8_t) (character - '0');
        }
        throw InvalidHexEncoding("No hexadecimal character: " + std::to_string(character));
    }

    Module *ModuleParser::parse(const std::string& str, const std::string& nameHint) {
        StringCharacterStream stream(str);

        SExprParser parser(stream);

        SExpr expr = parser.parse();

        ModuleParser moduleParser(expr, nameHint);
        Module* result = moduleParser.getParsedModule();

        return result;
    }

    void ModuleParser::parseExport(const SExpr& exportExpr) {
        module_->addExport(exportExpr[1].value(), module_->function(exportExpr[2].value()));
    }
}}
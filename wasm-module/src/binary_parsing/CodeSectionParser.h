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

#ifndef INTERPRETER_SECTIONPARSER_H
#define INTERPRETER_SECTIONPARSER_H


#include <Function.h>
#include <ModuleContext.h>
#include "ByteStream.h"
#include "FunctionParser.h"

namespace wasm_module { namespace binary {

    class CodeSectionParser {

        ByteStream &stream;

        std::vector<Function *> functions;
        std::vector<FunctionSignature> signatures;

        ModuleContext &context;

        void parseFunctions() {
            uint32_t numberOfFunctions = stream.popULEB128();

            for (uint32_t i = 0; i < numberOfFunctions; i++) {
                std::string functionName = stream.readCString();
                bool exported = stream.popULEB128() != 0;

                const Type* returnType = context.typeTable().getType(stream.popULEB128());

                uint32_t numberOfParameters = stream.popULEB128();
                std::vector<const Type*> parameters;
                for (uint32_t j = 0; j < numberOfParameters; j++) {
                    parameters.push_back(context.typeTable().getType(stream.popULEB128()));
                }

                uint32_t offset = stream.popULEB128();
                FunctionSignature signature = FunctionSignature(context.name(), functionName, returnType, parameters);
                signatures.push_back(signature);
                context.mainFunctionTable().addFunctionSignature(signature, signature.name());
            }

            for (int i = 0; i < numberOfFunctions; i++) {
                functions.push_back(FunctionParser::parse(context, signatures[i], stream));
            }
        }

    protected:
        CodeSectionParser(ModuleContext &context, ByteStream &stream)
                : stream(stream), context(context) {

        }


    public:
    };

}}

#endif //INTERPRETER_SECTIONPARSER_H

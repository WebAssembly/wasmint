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
#include <Variable.h>
#include <instructions/Instruction.h>
#include <instructions/InstructionSet.h>
#include <OpcodeTable.h>
#include <ModuleContext.h>
#include "ByteStream.h"

namespace wasm_module { namespace binary {

    ExceptionMessage(InvalidParameterType)

    class FunctionParser {
        ByteStream &stream;
        FunctionSignature functionSignature_;
        FunctionContext functionContext;

        ModuleContext &context_;
        Instruction *mainInstruction;

    protected:
        FunctionParser(ModuleContext &context, ByteStream &stream, FunctionSignature functionSignature)
                : context_(context), stream(stream), functionSignature_(functionSignature) {
        }

        void parse() {
            uint32_t numberOfLocals = stream.popULEB128();

            std::vector<const Type*> typeOfLocals;
            for (uint32_t i = 0; i < numberOfLocals; i++) {
                uint32_t typeData = stream.popULEB128();
                typeOfLocals.push_back(context_.typeTable().getType(typeData));
            }
            functionContext = FunctionContext(context_.name(), functionSignature_.name(), functionSignature_.returnType(),
                                              functionSignature_.parameters(), typeOfLocals);

            mainInstruction = parseInstruction();
        }

        Instruction *parseInstruction() {
            uint32_t opcode = stream.popULEB128();
            Instruction *instruction = InstructionSet::getInstruction(
                    context_.opcodeTable().getInstruction(opcode), stream, context_, functionContext);

            std::vector<Instruction *> children;

            for (uint32_t args = 0; args < instruction->childrenTypes().size(); args++) {
                Instruction *child = parseInstruction();

                const Type* expectedChildType = instruction->childrenTypes().at(args);
                if (expectedChildType != Void::instance()) {
                    if (expectedChildType != child->returnType()) {
                        throw InvalidParameterType(
                                std::string("Error while parsing parameter ") + std::to_string(args + 1) +
                                std::string(" for instruction ") + instruction->name() +
                                std::string(". Expected type ") + expectedChildType->name() +
                                " but got " + instruction->returnType()->name() +
                                std::string(". Parameter instruction that delivered wrong type was ") + child->name()
                        );
                    }
                }

                if (child->returnType())
                    children.push_back(child);
            }

            instruction->children(children);
            return instruction;
        }

        Function *getParsedFunction() {
            return new Function(functionContext, mainInstruction);
        }

    public:
        static Function *parse(ModuleContext &context, FunctionSignature &signature, ByteStream &stream) {
            FunctionParser parser(context, stream, signature);
            parser.parse();
            return parser.getParsedFunction();
        }
    };

}}

#endif //WASMINT_FUNCTIONPARSER_H

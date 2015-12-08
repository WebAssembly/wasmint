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

#ifndef WASMINT_FUNCTIONTABLEPARSER_H
#define WASMINT_FUNCTIONTABLEPARSER_H


#include <FunctionTable.h>

namespace wasm_module { namespace binary {

    class FunctionTableParser {

        ByteStream &stream;
        FunctionTable functionTable;

    protected:
        FunctionTableParser(ByteStream &stream) : stream(stream) {
        }

        void parse() {
            uint32_t numberOfFunctions = stream.popULEB128();
            for (uint32_t i = 0; i < numberOfFunctions; i++) {
                // TODO fix this: functionTable.addFunctionSignature(FunctionSignature());
            }
        }

        FunctionTable getParsedTable() {
            return functionTable;
        }

    public:
        static FunctionTable parse(ByteStream &stream) {
            FunctionTableParser parser(stream);
            parser.parse();
            return parser.getParsedTable();
        }
    };

}}

#endif //WASMINT_FUNCTIONTABLEPARSER_H

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

#ifndef WASMINT_MODULEPARSER_H
#define WASMINT_MODULEPARSER_H

#include <Module.h>
#include "CharacterStream.h"
#include "SExpr.h"

namespace wasm_module { namespace sexpr {


    ExceptionMessage(UnknownModuleChild)
    ExceptionMessage(MalformedImportStatement)
    ExceptionMessage(MissingModuleKeyword)
    ExceptionMessage(UnknownImportExpressionChild)
    ExceptionMessage(MultipleReturnTypesInImport)

    ExceptionMessage(MalformedMemoryStatement)
    ExceptionMessage(InvalidHexEncoding)

    class ModuleParser {

        Module* module_  = new Module();

        uint8_t hexToUint8(char character) const;
        uint8_t decodeHexEscape(char c1, char c2) const;

        std::vector<uint8_t> decodeEscapedString(const std::string& str) const;

        void parseFunctionType(const SExpr& functionTypeExpr);
        void parseIndirectCallTable(const SExpr &functionTypeTableExpr);

        void parseImport(const SExpr& importExpr);

        void parseMemory(const SExpr&memoryExpr);

        ModuleParser(const SExpr& moduleExpr, const std::string& nameHint);

        Module* getParsedModule() {
            return module_;
        }

    public:

        static Module* parse(const SExpr& expr) {
            ModuleParser parser(expr, "unnamedModule");
            return parser.getParsedModule();
        }

        static Module* parse(const std::string& str, const std::string& nameHint = "unnamedModule");
    };

}}


#endif //WASMINT_MODULEPARSER_H

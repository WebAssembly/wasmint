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


#ifndef WASMINT_MODULELOADER_H
#define WASMINT_MODULELOADER_H

#include <sexpr_parsing/ModuleParser.h>
#include <fstream>
#include <sexpr_parsing/SExprParser.h>
#include <sexpr_parsing/FileCharacterStream.h>
#include "Module.h"

namespace wasm_module {
    class ModuleLoader {
    public:
        static Module* loadFromFile(const std::string& filePath) {
            sexpr::FileCharacterStream stream(filePath);

            sexpr::SExpr expr = sexpr::SExprParser(stream).parse(true);
            Module* result = sexpr::ModuleParser::parse(expr[0]);
            return result;
        }
    };
}


#endif //WASMINT_MODULELOADER_H

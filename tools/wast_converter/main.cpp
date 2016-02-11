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

#include <sexpr_parsing/SExpr.h>
#include <sexpr_parsing/SExprParser.h>
#include <Module.h>

using namespace wasm_module;
using namespace wasm_module::sexpr;

int main(int argc, char** argv) {

    std::vector<std::string> args;
    args.resize((std::size_t) argc);
    for (std::size_t i = 0; i < argc; i++)
        args[i] = std::string(argv[i]);

    SExpr fileExpr = SExprParser::parseFile(args.at(1));

    std::unique_ptr<Module> module;

    for (const SExpr& child : fileExpr.children()) {
        const std::string& firstValue = child[0].value();
        if (firstValue == "module") {

        }
    }
}
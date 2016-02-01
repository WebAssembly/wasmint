#include "TestRunner.h"

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
#include <string>
#include <fstream>
#include <streambuf>
#include <sexpr_parsing/SExprParser.h>
#include <iostream>
#include <iomanip>
#include <builtins/SpectestModule.h>

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    std::ostream& operator<<(std::ostream& os, Code code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }
}



int main(int argc, char** argv) {

    std::string modulePath;
    std::string moduleContent;
    std::string expectedOutput;

    if (argc <= 1) {
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    } else if (argc <= 2) {
        modulePath = argv[1];
    } else {
        std::cerr << "Too many arguments" << std::endl;
        return 1;
    }

    std::size_t found = modulePath.find_last_of("/\\");

    std::string directory = modulePath.substr(0, found);
    std::string filename = modulePath.substr(found + 1);

    std::string expectedOutputPath = directory + "/expected-output/" + filename + ".log";

    {
        std::ifstream stream(modulePath);
        moduleContent = std::string((std::istreambuf_iterator<char>(stream)),
                        std::istreambuf_iterator<char>());
        stream.close();
    }

    {
        std::ifstream stream(expectedOutputPath);
        expectedOutput = std::string((std::istreambuf_iterator<char>(stream)),
                                    std::istreambuf_iterator<char>());
        stream.close();
    }

    wasm_module::sexpr::SExpr expr = wasm_module::sexpr::SExprParser::parseString(moduleContent);

    std::cout << std::left << std::setw(73) << argv[1] << std::setw(0) << " ";
    std::cout.flush();

    TestRunner testRunner;
    if (!testRunner.run(expr)) {
        std::cout << "[" << Color::FG_RED << "  FAIL  " << Color::FG_DEFAULT << "]" << std::endl;
        return 1;
    }

    std::string actualOutput = wasmint::SpectestModule::stdout().str();
    if (!expectedOutput.empty() && actualOutput != expectedOutput) {
        std::cout << "[" << Color::FG_RED << " STDOUT " << Color::FG_DEFAULT << "]" << std::endl;
        std::cout << "Output was:" << actualOutput << std::endl;
        return 1;
    } else {
        std::cout << "[" << Color::FG_GREEN << "   OK   " << Color::FG_DEFAULT << "]" << std::endl;
    }
}
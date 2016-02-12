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
#include <sexpr_parsing/ModuleParser.h>
#include <boost/filesystem.hpp>


using namespace wasm_module;
using namespace wasm_module::sexpr;

class WastConverter {
    std::unique_ptr<Module> currentModule_;
    SExpr currentModuleExpr_;

    SExpr mainFunction_;

    std::string outDir_;
    std::string baseName_;
    std::size_t outIndex_ = 0;

    std::string generateOutFile(const std::string& directory) {
        std::string directoryPath = outDir_ + "/" + directory;
        boost::filesystem::create_directories(directoryPath);
        outIndex_++;
        return directoryPath + "/" + baseName_ + "_" + std::to_string(outIndex_) + ".wasm";
    }

    std::string generatePositiveOutFile(const std::string& directory) {
        std::string directoryPath = outDir_ + "/" + directory;
        boost::filesystem::create_directories(directoryPath);
        return directoryPath + "/" + baseName_ + ".wasm";
    }

    void setCurrentModule(const SExpr& expr) {
        currentModule_.reset(ModuleParser::parse(expr));
        currentModuleExpr_ = expr;

        mainFunction_ = SExprParser::parseString("func $main");
    }

    SExpr convertInvoke(const SExpr& invoke) {
        std::string resultString = "call " + currentModule_->exportedFunction(invoke[1].value())->name() + " ";
        for (std::size_t i = 2; i < invoke.children().size(); i++) {
            resultString += invoke[i].toString();
            resultString += " ";
        }
        SExpr result = SExprParser::parseString(resultString);
        return result;
    }

public:
    WastConverter(const std::string& outDir, const boost::filesystem::path& path) : outDir_(outDir) {
        baseName_ = path.stem().string();

        SExpr fileExpr = SExprParser::parseFile(path.string());
        for (const SExpr& child : fileExpr.children()) {
            const std::string& firstValue = child[0].value();
            if (firstValue == "module") {
                setCurrentModule(child);
            } else if (firstValue == "assert_invalid") {
                std::ofstream outStream(generateOutFile("invalid"));
                outStream << child[1].toString();
                outStream.close();
            } else if (firstValue == "assert_return_nan") {
                SExpr functionCall = convertInvoke(child[1]);
                SExpr newTest = SExprParser::parseString("if (i32.eq (i32.and () " + functionCall.toString() + ") (nop) (unreachable)");
                mainFunction_.addChild(newTest);
            } else if (firstValue == "assert_return") {
                if (child.children().size() == 2) {
                    SExpr functionCall = convertInvoke(child[1]);

                    mainFunction_.addChild(functionCall);
                } else {
                    std::string compareType = child[2][0].value().substr(0, 3);

                    SExpr functionCall = convertInvoke(child[1]);

                    SExpr newTest = SExprParser::parseString("if (" + compareType + ".eq " + functionCall.toString() + " " + child[2].toString() +") (nop) (unreachable)");
                    mainFunction_.addChild(newTest);
                }
            } else if (firstValue == "assert_trap") {
                SExpr functionCall = convertInvoke(child[1]);

                SExpr mainFunctionWithTrap = mainFunction_;
                mainFunctionWithTrap.addChild(functionCall);

                SExpr result = currentModuleExpr_;
                result.addChild(mainFunctionWithTrap);

                std::ofstream outStream(generateOutFile("trap"));
                outStream << result.toString();
                outStream.close();
            } else if (firstValue == "invoke") {
                SExpr functionCall = convertInvoke(child[1]);

                mainFunction_.addChild(functionCall);
            } else {
                std::cerr << "Can't handle assert " << child.toString() << std::endl;
            }
        }
        SExpr positiveModule = currentModuleExpr_;
        positiveModule.addChild(mainFunction_);

        std::ofstream outStream(generatePositiveOutFile("positive"));
        outStream << positiveModule.toString();
        outStream.close();
    }
};

int main(int argc, char** argv) {

    std::vector<std::string> args;
    args.resize((std::size_t) argc);
    for (std::size_t i = 0; i < argc; i++)
        args[i] = std::string(argv[i]);

    // at least 3 args: ./program outdir file1 file2 ...
    assert(args.size() >= 3);

    std::string outDir = args.at(1);

    for (std::size_t i = 2; i < args.size(); i++) {
        WastConverter converter(outDir, args.at(i));
    }

}
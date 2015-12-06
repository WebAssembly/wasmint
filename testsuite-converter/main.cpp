
#include <iosfwd>
#include <string>
#include <fstream>
#include <sexpr_parsing/SExpr.h>
#include <sexpr_parsing/SExprParser.h>
#include <iostream>
#include <map>
#include <list>
#include <typeinfo>
#include <boost/filesystem.hpp>
#include <iomanip>
#include <assert.h>

using namespace wasm_module;
using namespace wasm_module::sexpr;


class UnknownAssertValue : public std::exception {};
class NoModuleWithExport : public std::exception {};

class ModuleWrapper {
public:
    SExpr moduleExpr_;
    std::map<std::string, std::string> exports;

    SExpr positiveExpr_;
    std::vector<SExpr> negativeTrapExprs_;

    ModuleWrapper(const SExpr& moduleExpr) {
        moduleExpr_ = moduleExpr;

        std::list<std::size_t> positionsToRemove;
        for (std::size_t i = 0; i < moduleExpr_.children().size(); i++) {
            if (moduleExpr[i].hasChildren() && moduleExpr_[i][0].value() == "export") {
                positionsToRemove.push_back(i);
                exports[moduleExpr[i][1].value()] = moduleExpr[i][2].value();
            }
        }
        while (!positionsToRemove.empty()) {
            moduleExpr_.removeChild(positionsToRemove.back());
            positionsToRemove.pop_back();
        }
    }

    bool hasPositiveChecks() {
        return positiveExpr_.hasChildren();
    }

    void addPositiveCheck(const SExpr& expr) {
        positiveExpr_.addChild(expr);
    }

    void addNegativeTrapCheck(const SExpr& expr) {
        negativeTrapExprs_.push_back(expr);
    }

    void addMain(SExpr& module, const SExpr& mainContent) {
        SExpr mainExpr;
        mainExpr.addChild("func");
        mainExpr.addChild("main");
        mainExpr.addChild(mainContent);
        module.addChild(mainExpr);
    }

    SExpr getPositiveTestSource() {
        SExpr result = moduleExpr_;

        addMain(result, positiveExpr_);

        return result;
    }

    std::vector<SExpr> getNegativeTrapSources() {
        std::vector<SExpr> result;
        result.reserve(negativeTrapExprs_.size());

        for (const SExpr& expr : negativeTrapExprs_) {
            SExpr output = moduleExpr_;
            addMain(output, expr);
            result.push_back(output);
        }

        return result;
    }

    bool hasExport(std::string expo) {
        return exports.find(expo) != exports.end();
    }

    std::string getExport(std::string expo) {
        return exports.find(expo)->second;
    }
};

class WastTestTransformer {
    std::string baseName;
    std::vector<ModuleWrapper> modules;
    sexpr::SExpr mainExpr;
    unsigned numberOfTrapTests = 0;
    unsigned numberOfInvalidTests = 0;
    std::vector<SExpr> invalidExprs_;

    void writeSExpr(std::string directoryPath, const SExpr& sexpr, std::string fileName) {

        boost::filesystem::path directory(directoryPath);
        boost::filesystem::create_directories(directory);

        boost::filesystem::path absolutePath = directory / (fileName + ".wasm");

        std::ofstream out(absolutePath.string());

        out << sexpr.toString();
        out.close();
    }

public:
    WastTestTransformer(const std::string& wastPath) {
        boost::filesystem::path p(wastPath);

        baseName = p.stem().string();

        std::ifstream moduleFile(wastPath);
        std::string moduleData((std::istreambuf_iterator<char>(moduleFile)),
                               std::istreambuf_iterator<char>());
        sexpr::CharacterStream stream(moduleData);

        mainExpr = sexpr::SExprParser(stream).parse(true);

        for (const SExpr& expr : mainExpr.children()) {
            if (expr.hasChildren()) {
                if (expr[0].value() == "module") {
                    modules.push_back(ModuleWrapper(expr));
                }
            }
        }
    }

    int countStringOccurencesInString(const std::string& hay, const std::string& needle) {
        if (needle == "")
            return 0;

        int occurrences = 0;
        std::string::size_type start = 0;

        while ((start = hay.find(needle, start)) != std::string::npos) {
            ++occurrences;
            start += needle.length();
        }
        return occurrences;
    }

    ModuleWrapper& getModule(const std::string& exportName, const std::string& expectedReturnType) {
        std::vector<ModuleWrapper*> possibleResults;
        for(ModuleWrapper& moduleWrapper : modules) {
            if (moduleWrapper.hasExport(exportName)) {
                possibleResults.push_back(&moduleWrapper);
            }
        }

        if (possibleResults.empty()) {
            std::cerr << "Couldn't find any module that exports: " << exportName << std::endl;
            throw NoModuleWithExport();
        } else {
            ModuleWrapper* bestMatch = possibleResults.front();
            int amountOfDataTypeOccurences = -1;
            for (ModuleWrapper* module : possibleResults) {
                int occurrences = countStringOccurencesInString(module->moduleExpr_.toString(), expectedReturnType);
                if (occurrences > amountOfDataTypeOccurences) {
                    bestMatch = module;
                    amountOfDataTypeOccurences = occurrences;
                }
            }
            return *bestMatch;
        }
    }

    ModuleWrapper& invokeToCall(SExpr&invoke, std::string dataType = "") {
        ModuleWrapper& wrapper = getModule(invoke[1].value(), dataType);

        std::string functionName = wrapper.getExport(invoke[1].value());

        invoke[0] = SExpr("call");
        invoke[1] = SExpr(functionName);

        return wrapper;
    }

    std::string createUniqueFileSuffix(int index, std::size_t totalTests) {
        if (totalTests == 1) {
            return "";
        }
        std::stringstream basenameSuffix;
        basenameSuffix << "_" << std::setw((int) log10(totalTests) + 1) << std::setfill('0') << std::to_string(index);
        return basenameSuffix.str();
    }

    void generateOutput() {
        int testNumber = 1;
        for (SExpr expr : mainExpr.children()) {
            if (!expr.hasChildren())
                continue;
            if (expr[0].value() == "invoke") {
                ModuleWrapper& wrapper = invokeToCall(expr);
                wrapper.addPositiveCheck(expr);
            } else if (expr[0].value() == "assert_return") {
                SExpr invokeExpr = expr[1];

                std::string dataType;

                if (expr.children().size() <= 2) {
                    std::cerr << "warning - can't handle this assert_return: " << expr.toString() << std::endl;
                } else {
                    if (expr[2][0].value() == "i32.const") {
                        dataType = "i32";
                    } else if (expr[2][0].value() == "i64.const") {
                        dataType = "i64";
                    } else if (expr[2][0].value() == "f32.const") {
                        dataType = "f32";
                    } else if (expr[2][0].value() == "f64.const") {
                        dataType = "f64";
                    } else {
                        std::cerr << expr[2][0].value() << std::endl;
                        throw UnknownAssertValue();
                    }

                    ModuleWrapper& wrapper = invokeToCall(invokeExpr, dataType);

                    if (dataType[0] == 'i') {
                        SExpr ifExpr;
                        ifExpr.addChild("if_else");

                        SExpr eqExpr;
                        eqExpr.addChild(dataType + ".eq");
                        eqExpr.addChild(invokeExpr);
                        eqExpr.addChild(expr[2]);

                        ifExpr.addChild(eqExpr);
                        ifExpr.addChild();
                        ifExpr.addChild(SExprParser::parseString("(unreachable)"));
                        wrapper.addPositiveCheck(ifExpr);
                    } else {
                        if (dataType == "f32") {
                            SExpr ifExpr = SExprParser::parseString("(if_else (i32.eq (i32.reinterpret/f32) (i32.reinterpret/f32)) () (unreachable) )");
                            ifExpr[0][1][1].addChild(invokeExpr);
                            ifExpr[0][1][2].addChild(expr[2]);
                            wrapper.addPositiveCheck(ifExpr);
                        } else if (dataType == "f64") {
                            SExpr ifExpr = SExprParser::parseString("(if_else (i64.eq (i64.reinterpret/f64) (i64.reinterpret/f64)) () (unreachable) )");
                            ifExpr[0][1][1].addChild(invokeExpr);
                            ifExpr[0][1][2].addChild(expr[2]);
                            wrapper.addPositiveCheck(ifExpr);
                        } else {
                            assert(false);
                        }
                    }

                }

            } else if (expr[0].value() == "assert_return_nan") {
                // TODO
            } else if (expr[0].value() == "assert_trap") {
                numberOfTrapTests++;

                ModuleWrapper& wrapper = invokeToCall(expr[1]);
                wrapper.addNegativeTrapCheck(expr);

            } else if (expr[0].value() == "assert_invalid") {
                numberOfInvalidTests++;
                invalidExprs_.push_back(expr[1]);
            } else if (expr[0].value() == "module") {
                // TODO
                testNumber--;
            } else {
                std::cerr << "warning - unknown top level expr: " << expr.toString() << std::endl;
            }
            testNumber++;
        }

        int i = 1;
        for(ModuleWrapper& moduleWrapper : modules) {
            SExpr output = moduleWrapper.getPositiveTestSource();
            writeSExpr("positive/", output, baseName + createUniqueFileSuffix(i, modules.size()));
            i++;
        }

        i = 1;
        for(ModuleWrapper& moduleWrapper : modules) {
            for (const SExpr& trapSource : moduleWrapper.getNegativeTrapSources()) {
                writeSExpr("negative/trap/", trapSource.toString(), baseName + createUniqueFileSuffix(i, numberOfTrapTests));
                i++;
            }
        }

        i = 1;
        for (const SExpr& invalidExpr : invalidExprs_) {
            writeSExpr("negative/invalid/", invalidExpr.toString(), baseName + createUniqueFileSuffix(i, numberOfInvalidTests));
            i++;
        }
    }

};

int main(int argv, char** argc) {

    if (argv <= 1) {
        std::cerr << "testsuite-converter needs at least one path to a wast test file as an argument" << std::endl;
        return 1;
    }

    for (int i = 1; i < argv; i++) {
        std::string modulePath = argc[i];

        std::cout << "Processing " << modulePath << std::endl;
        try {

            WastTestTransformer transformer(modulePath);
            transformer.generateOutput();
            std::cout << "Done!" << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Got exception: " << typeid(ex).name() << " - what(): " << ex.what() << std::endl;
        }

    }

    return 0;
}
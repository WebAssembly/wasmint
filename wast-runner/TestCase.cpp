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


#include <sexpr_parsing/ModuleParser.h>
#include <assert.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>
#include <cmath>
#include "TestCase.h"

bool TestCase::run(wasmint::WasmintVM& vm) {
    if (type_ == Type::Invalid) {
        // handles in the run method without vm parameter
        assert(false);
    } else if (type_ == Type::AssertTrap) {
        wasmint::VMState stateCopy = vm.state();
        const wasm_module::Function* function = getExportedFunction(vm);
        vm.startAtFunction(*function, parameters_, false);
        vm.stepUntilFinished(false);
        if (!vm.gotTrap()) {
            std::cerr << "Didn't got a trap during test:\n" << testCaseExpr_.toString(4) << std::endl;
            vm.state() = stateCopy;
            return false;
        }
        if (vm.trapReason() != expectedTrapReason_) {
            std::cerr << "Got wrong trap reason in test case: '" << vm.trapReason() << "'\n" << testCaseExpr_.toString(4) << std::endl;
            vm.state() = stateCopy;
            return false;
        }
        vm.state() = stateCopy;
        return true;
    } else if (type_ == Type::Invoke) {
        const wasm_module::Function* function = getExportedFunction(vm);
        if (function == nullptr)
            return false;
        vm.startAtFunction(*function, parameters_, false);
        vm.stepUntilFinished(false);
        if (vm.gotTrap()) {
            std::cerr << "Failed test due to unexpected trap: " << vm.trapReason() << "\n" << testCaseExpr_.toString(4) << std::endl;
            return false;
        }
        return true;
    } else if (type_ == Type::AssertReturn) {
        const wasm_module::Function* function = getExportedFunction(vm);
        vm.startAtFunction(*function, parameters_, false);
        vm.stepUntilFinished(false);
        if (vm.gotTrap()) {
            std::cerr << "Failed test due to unexpected trap: " << vm.trapReason() << "\n" << testCaseExpr_.toString(4) << std::endl;
            return false;
        }
        if (vm.state().thread().result() == expectedResult_) {
            return true;
        } else {
            std::cerr << "Expected " << expectedResult_.toString() << " but got " <<
                    vm.state().thread().result().toString() << "\n" << testCaseExpr_.toString(4) << std::endl;
        }
    } else if (type_ == Type::AssertReturnNan) {
        const wasm_module::Function* function = getExportedFunction(vm);
        vm.startAtFunction(*function, parameters_, false);
        vm.stepUntilFinished(false);
        if (vm.gotTrap()) {
            std::cerr << "Failed test due to unexpected trap: " << vm.trapReason() << "\n" << testCaseExpr_.toString(4) << std::endl;
            return false;
        }
        wasm_module::Variable result = vm.state().thread().result();
        if (&result.type() == wasm_module::Float32::instance()) {
            if (std::isnan(result.float32())) {
                return true;
            } else {
                std::cerr << "Failed test as result is not NaN: " << result.toString() << "\n" << testCaseExpr_.toString(4) << std::endl;
                return false;
            }
        } else if (&result.type() == wasm_module::Float64::instance()) {
            if (std::isnan(result.float64())) {
                return true;
            } else {
                std::cerr << "Failed test as result is not NaN: " << result.toString() << "\n" << testCaseExpr_.toString(4) << std::endl;
                return false;
            }
        } else {
            std::cerr << "Failed test as result is not a floating point number: " << result.toString() << "\n" << testCaseExpr_.toString(4) << std::endl;
            return false;
        }
    } else {
        assert(false);
    }
}

TestCase::TestCase(const wasm_module::sexpr::SExpr& expr) {
    testCaseExpr_ = expr;
    if (expr[0].value() == "assert_invalid") {
        type_ = Type::Invalid;
        invalidModuleExpr_ = expr[1];
    } else if (expr[0].value() == "assert_return") {
        type_ = Type::AssertReturn;
        parseInvoke(expr[1]);
        if (expr.children().size() > 2)
            parseExpectedResult(expr[2]);
    } else if (expr[0].value() == "assert_return_nan") {
        type_ = Type::AssertReturnNan;
        parseInvoke(expr[1]);
    } else if (expr[0].value() == "assert_trap") {
        type_ = Type::AssertTrap;
        parseInvoke(expr[1]);
        expectedTrapReason_ = expr[2].value();
    } else if (expr[0].value() == "invoke") {
        type_ = Type::Invoke;
        parseInvoke(expr);
    } else {
        assert(false);
    }
}

void TestCase::parseExpectedResult(const wasm_module::sexpr::SExpr& expr) {
    expectedResult_ = parseVariable(expr);
}

void TestCase::parseInvoke(const wasm_module::sexpr::SExpr& expr) {
    assert(expr[0].value() == "invoke");
    functionToCall_ = expr[1].value();
    for (std::size_t i = 2; i < expr.children().size(); i++) {
        parameters_.push_back(parseVariable(expr[i]));
    }
}

wasm_module::Variable TestCase::parseVariable(const wasm_module::sexpr::SExpr& expr) {
    wasm_module::Variable result;
    if (expr[0].value() == "i32.const") {
        result = wasm_module::Variable(wasm_module::Int32::instance());
    } else if (expr[0].value() == "i64.const") {
        result = wasm_module::Variable(wasm_module::Int64::instance());
    } else if (expr[0].value() == "f32.const") {
        result = wasm_module::Variable(wasm_module::Float32::instance());
    } else if (expr[0].value() == "f64.const") {
        result = wasm_module::Variable(wasm_module::Float64::instance());
    } else {
        assert(false);
    }
    result.type().parse(expr[1].value(), result.value());
    return result;
}

const wasm_module::Function *TestCase::getExportedFunction(wasmint::WasmintVM& vm) {
    for (const wasm_module::Module* module : vm.modules()) {
        try {
            const wasm_module::Function* exportedFunction = module->exportedFunction(functionToCall_);
            if (exportedFunction->parameters().size() == parameters_.size()) {
                bool parametersFit = true;
                for (std::size_t i = 0; i < parameters_.size(); i++) {
                    if (exportedFunction->parameters()[i] != &parameters_[i].type()) {
                        parametersFit = false;
                        break;
                    }
                }
                if (parametersFit)
                    return exportedFunction;
            }
        } catch (const wasm_module::NoExportWithName& ex) {
            // pass
        }
    }
    throw CouldntFindExportedFunction();
}

bool TestCase::run() {
    if (type_ != Type::Invalid) {
        // we only handle assert invalid here. Everything else is handles in the run method with the vm parameter
        assert(false);
    }
    try {
        wasm_module::sexpr::ModuleParser::parse(invalidModuleExpr_);
        std::cerr << "Failed to find parse error in:\n" << invalidModuleExpr_.toString(4) << std::endl;
        return false;
    } catch (const std::exception& ex) {
        // pass
        return true;
    }
}
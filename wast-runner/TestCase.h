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


#ifndef WASMINT_TESTCASE_H
#define WASMINT_TESTCASE_H


#include <sexpr_parsing/SExpr.h>
#include <interpreter/WasmintVM.h>

ExceptionMessage(CouldNotFindExportedFunction);

class TestCase {


    enum class Type {
        Invalid,
        AssertTrap,
        Invoke,
        AssertReturn,
        AssertReturnNan
    };

    Type type_;

    wasm_module::sexpr::SExpr testCaseExpr_;

    wasm_module::sexpr::SExpr invalidModuleExpr_;
    wasm_module::Variable expectedResult_;

    std::string expectedTrapReason_;

    std::string functionToCall_;
    std::vector<wasm_module::Variable> parameters_;

    void parseInvoke(const wasm_module::sexpr::SExpr& expr);

    void parseExpectedResult(const wasm_module::sexpr::SExpr& expr);

    wasm_module::Variable parseVariable(const wasm_module::sexpr::SExpr& expr);

    const wasm_module::Function * getExportedFunction(wasmint::WasmintVM& vm);

public:
    TestCase(const wasm_module::sexpr::SExpr& expr);

    bool needsVM() {
        return type_ != Type::Invalid;
    }

    bool run();

    bool run(wasmint::WasmintVM& vm);


};


#endif //WASMINT_TESTCASE_H

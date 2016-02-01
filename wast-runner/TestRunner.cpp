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


#include <assert.h>
#include <ModuleLoader.h>
#include <builtins/SpectestModule.h>
#include "TestRunner.h"

bool TestRunner::run(const wasm_module::sexpr::SExpr& expr) {
    using namespace wasm_module::sexpr;

    for (const SExpr& child : expr.children()) {
        if (child.hasChildren()) {
            std::string firstChild = child[0].value();
            if (firstChild == "module") {
                wasm_module::Module* module = wasm_module::sexpr::ModuleParser::parse(child);
                vm.reset(new wasmint::WasmintVM());
                vm->loadModule(*wasmint::SpectestModule::create(), true);
                vm->loadModule(*module, true);
            } else {
                TestCase testCase(child);
                if (testCase.needsVM()) {
                    if (!testCase.run(*vm)) {
                        return false;
                    }
                } else {
                    if (!testCase.run()) {
                        return false;
                    }
                }
            }
        } else {
            assert(false);
        }
    }
    return true;
}

TestRunner::~TestRunner() {
}
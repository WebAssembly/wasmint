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


#include <ModuleLoader.h>
#include "WasmintVM.h"

void wasmint::WasmintVM::loadModule(const std::string &path) {
    wasm_module::Module* module = wasm_module::ModuleLoader::loadFromFile(path);
    loadModule(*module, true);
}

void wasmint::WasmintVM::loadModuleFromData(const std::string &moduleContent) {
    wasm_module::Module* module = wasm_module::sexpr::ModuleParser::parse(moduleContent);
    loadModule(*module, true);
}

void wasmint::WasmintVM::startAtFunction(const wasm_module::Function& function, bool enableHistory) {
    linkModules();
    for (std::size_t i = 0; i < functions_.size(); i++) {
        if (&functions_[i].function() == &function) {
            state_.heap().removeObserver();
            state_.heap().attachObserver(history_);
            state_.startAtFunction(this, i);
            if (enableHistory) {
                startHistoryRecording();
            }
            return;
        }
    }
    throw std::domain_error("Can't find compiled function with name " + function.name());
}

void wasmint::WasmintVM::startAtFunction(const wasm_module::Function& function, const std::vector<wasm_module::Variable>& parameters, bool enableHistory) {
    linkModules();
    for (std::size_t i = 0; i < functions_.size(); i++) {
        if (&functions_[i].function() == &function) {
            state_.heap().removeObserver();
            state_.heap().attachObserver(history_);
            state_.startAtFunction(this, i, parameters);
            if (enableHistory) {
                startHistoryRecording();
            }
            return;
        }
    }
    throw std::domain_error("Can't find compiled function with name " + function.name());
}
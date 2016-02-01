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


#include <Module.h>
#include "FunctionList.h"
#include "WasmDbg.h"

void FunctionList::render() {
    int y = 0;
    std::size_t functionIndex = 0;
    const wasm_module::Module* module = nullptr;
    for (const wasm_module::Function* function : functions_) {
        if (&function->module() != module) {
            module = &function->module();
            print(0, y, "Module " + module->name());
            y++;
        }
        if (function == selectedFunction_)
            print(0, y, " -> " + function->name(), A_REVERSE);
        else
            print(0, y, " -> " + function->name());

        y++;
        functionIndex++;
    }
}

FunctionList::FunctionList(int x, int y, int w, int h) : Widget(x, y, w, h) {
    setBorder(1);
    setName("Functionlist");

    instructionViewer_.reset(new InstructionViewer(x, y, w, h));
    instructionViewer_->hide();
    addChild(instructionViewer_.get());

    for (const wasm_module::Module* module : WasmDbg::instance()->vm().modules()) {
        for (const wasm_module::Function* function : module->functions()) {
            functions_.push_back(function);
        }
    }
    if (!functions_.empty()) {
        selectedFunction_ = functions_.front();
    }
}

bool FunctionList::handleCharacter(int c) {
    if (functions_.empty()) {
        selectedFunction_ = nullptr;
    } else {
        if (c == KEY_UP) {
            selectedFunction_ = functions_[(selectedIndex() - 1) % functions_.size()];
        }
        if (c == KEY_DOWN) {
            selectedFunction_ = functions_[(selectedIndex() + 1) % functions_.size()];
        }
        if (c == KEY_RIGHT) {
            selectedFunction_ = functions_[(selectedIndex() + 1) % functions_.size()];
            instructionViewer_->show();
        }
    }
    return true;
}
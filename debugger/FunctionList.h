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


#ifndef WASMINT_FUNCTIONLIST_H
#define WASMINT_FUNCTIONLIST_H


#include "Widget.h"
#include "InstructionViewer.h"
#include <memory>

class FunctionList : public Widget {

    std::unique_ptr<InstructionViewer> instructionViewer_;
    std::vector<const wasm_module::Function*> functions_;
    const wasm_module::Function* selectedFunction_ = nullptr;

    std::size_t selectedIndex() {
        for (std::size_t i = 0; i < functions_.size(); i++) {
            if (functions_[i] == selectedFunction_)
                return i;
        }
        return 0;
    }

public:
    FunctionList(int x, int y, int w, int h);

    virtual void render();

    virtual bool handleCharacter(int c);

};


#endif //WASMINT_FUNCTIONLIST_H

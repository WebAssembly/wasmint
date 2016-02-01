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


#ifndef WASMINT_VMVIEWER_H
#define WASMINT_VMVIEWER_H


#include <interpreter/WasmintVM.h>
#include <sexpr_parsing/ModuleParser.h>
#include "Widget.h"
#include "MemoryViewer.h"
#include "InstructionViewer.h"
#include "FunctionList.h"

class VMViewer : public Widget {

    wasmint::WasmintVM* vm;

    std::unique_ptr<MemoryViewer> memoryViewer;
    std::unique_ptr<FunctionList> functionList;

    enum class Tab {
        Memory,
        Instructions
    };
    Tab tab = Tab::Memory;

public:
    VMViewer(int width, int height);

    virtual bool handleCharacter(int c) override {
        if (c == 'm') {
            tab = Tab::Memory;
            functionList->hide();
            memoryViewer->show();
            return true;
        }
        if (c == 'i') {
            tab = Tab::Instructions;
            functionList->show();
            memoryViewer->hide();
            return true;
        }
        return false;
    }

    virtual void render() override {

        memoryViewer->draw();
        functionList->draw();

    }

};


#endif //WASMINT_VMVIEWER_H

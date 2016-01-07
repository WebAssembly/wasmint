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

class VMViewer : public Widget {

    wasmint::WasmintVM* vm;

    wasmint::Heap heap;
    MemoryViewer memoryViewer;
    InstructionViewer instructionViewer;

public:
    VMViewer() : Widget(1, 1, 80, 20) {

        heap.grow(1000);
        heap.setByte(0, 0xFA);

        memoryViewer.setHeap(&heap);

        wasm_module::Module* positiveModule = wasm_module::sexpr::ModuleParser::parse(
                "module (func main "
                        "(if_else (i32.const 0) (unreachable) ())"
                        "(if_else (i32.const 1) (unreachable) ())"
                        "(if_else (i32.const 2) (unreachable) ())"
                        ")");

        instructionViewer.setInstruction(positiveModule->functions().front()->mainInstruction());

        setBorder(1);
        setName("VM");

        addChild(&memoryViewer);
        addChild(&instructionViewer);
    }


    virtual bool handleCharacter(int c) {
        memoryViewer.handleCharacter(c);

        memoryViewer.draw();

        instructionViewer.handleCharacter(c);
        instructionViewer.draw();
        return true;
    }

};


#endif //WASMINT_VMVIEWER_H

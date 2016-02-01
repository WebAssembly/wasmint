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


#include "VMViewer.h"
#include "WasmDbg.h"

VMViewer::VMViewer(int width, int height) : Widget(0, 0, width, height) {
    memoryViewer.reset(new MemoryViewer(0, 0, width, height));
    functionList.reset(new FunctionList(0, 0, width, height));

    memoryViewer->setHeap(&WasmDbg::instance()->vm().heap());

    addChild(memoryViewer.get());
    addChild(functionList.get());

    tab = Tab::Instructions;
    functionList->show();
    memoryViewer->hide();
}
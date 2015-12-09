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


#include <cstdint>
#include <Module.h>
#include <types/Int32.h>
#include <interpreter/MachineState.h>
#include <sexpr_parsing/CharacterStream.h>
#include <sexpr_parsing/SExprParser.h>
#include <sexpr_parsing/ModuleParser.h>
#include <interpreter/Thread.h>
#include <assert.h>
#include "performance/QuickSortSource.h"

using namespace wasm_module;
using namespace wasm_module::sexpr;
using namespace wasmint;

int main() {

    MachineState environment;

    Module* positiveModule = ModuleParser::parse(quickSortSource);

    environment.useModule(*positiveModule, true);

    Thread& thread = environment.createThread().startAtFunction(positiveModule->name(), "main");
    thread.stepUntilFinished();

    Heap& heap = thread.getHeap(*positiveModule);

    uint8_t smallestValue = 0;
    for (std::size_t i = 0; i < heap.size(); i++) {
        uint8_t value = heap.getByte(i);
        assert (value >= smallestValue);
        smallestValue = value;
    }
}
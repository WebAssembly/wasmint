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
#include <interpreter/at/MachineState.h>
#include <sexpr_parsing/CharacterStream.h>
#include <sexpr_parsing/SExprParser.h>
#include <sexpr_parsing/ModuleParser.h>
#include <assert.h>
#include "QuickSortSource.h"
#include <chrono>
#include <iostream>
#include <interpreter/rm/RegisterMachine.h>

using namespace std;
using namespace std::chrono;

using namespace wasm_module;
using namespace wasm_module::sexpr;
using namespace wasmint;

void assertHeapSorted(Heap& heap) {
    uint8_t smallestValue = 0;
    for (std::size_t i = 0; i < heap.size(); i++) {
        uint8_t value = heap.getByte(i);
        if (value < smallestValue) {
            std::cout << "Heap not sorted at position " << i
                      << "! This means that quicksort was not proberly executed" << std::endl;
            abort();
        }
        smallestValue = value;
    }
}

void runATCore() {
    std::cout.setf( std::ios::fixed, std:: ios::floatfield );
    std::cout.precision(4);

    MachineState environment;

    Module* positiveModule = ModuleParser::parse(quickSortSource);

    environment.useModule(*positiveModule, true);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    InterpreterThread * thread = &environment.createThread().startAtFunction(positiveModule->name(), "main");
    thread->stepUntilFinished();
    if (thread->gotTrap()) {
        std::cerr << "thread got trap: " << thread->trapReason() << std::endl;
        abort();
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    int64_t duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "AT core took " << duration << " microseconds (" << (duration / 1000000.0) << " seconds)" << std::endl;


    Heap& heap = thread->getHeap(*positiveModule);

    assertHeapSorted(heap);
}

void runRMCore() {
    std::cout.setf( std::ios::fixed, std:: ios::floatfield );
    std::cout.precision(4);

    RegisterMachine registerMachine;

    Module* positiveModule = ModuleParser::parse(quickSortSource);

    registerMachine.useModule(*positiveModule, true);

    VMThread& thread = registerMachine.startAtFunction(*positiveModule->functions().back());
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    registerMachine.stepUntilFinished();

    if (thread.gotTrap()) {
        std::cerr << "thread got trap: " << thread.trapReason() << std::endl;
        abort();
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    int64_t duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "RM core took " << duration << " microseconds (" << (duration / 1000000.0) << " seconds)" << std::endl;


    Heap& heap = registerMachine.heap();

    assertHeapSorted(heap);
}

int main() {
    //runATCore();
    runRMCore();
}
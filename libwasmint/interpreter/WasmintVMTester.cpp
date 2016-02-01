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


#include "WasmintVMTester.h"

void wasmint::WasmintVMTester::stepUntilFinished() {
    vm_.state().instructionCounter(0);

    vm_.startHistoryRecording();


    VMState startState = vm_.state();
    while (true) {
        vm_.step();
        if (selectedIntermediateStates.size() < maxIntermediateStates
            && vm_.instructionCounter().multipleOf(intermediateStateInterval)) {
            selectedIntermediateStates[vm_.instructionCounter()] = vm_.state();
        }
        if (vm_.instructionCounter().multipleOf(5)) {
            vm_.history().addCheckpoint(vm_.state());
        }
        if (vm_.finished()) {
            break;
        }
    }
    VMState endState = vm_.state();

    for (auto& pair : selectedIntermediateStates) {
        vm_.simulateTo(pair.first);
        if (pair.second != vm_.state()) {
            throw FailedVMTest("Couldn't simulate to selected state");
        }
    }

    vm_.simulateTo(0);
    if (startState != vm_.state()) {
        throw FailedVMTest("Couldn't rollback to start state");
    }
    vm_.simulateTo(endState.instructionCounter());
    if (endState != vm_.state()) {
        throw FailedVMTest("Couldn't reproduce end state");
    }
}
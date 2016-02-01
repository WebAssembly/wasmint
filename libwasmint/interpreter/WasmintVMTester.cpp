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
    states_.push_back(vm_.state());
    while (true) {
        vm_.step();
        states_.push_back(vm_.state());
        if (vm_.finished()) {
            break;
        }
    }

    for (std::size_t i = states_.size() - 1; i > 0 ; i--) {
        if (states_[i] != vm_.state()) {
            throw FailedVMTest("Couldn't roll back state with counter " + std::to_string(i));
        }
        vm_.stepBack();
    }
    if (states_[0] != vm_.state()) {
        throw FailedVMTest("Couldn't roll back state with counter 0");
    }
    for (std::size_t i = 0; i < states_.size(); i++) {
        if (states_[i] != vm_.state()) {
            throw FailedVMTest("Couldn't rerun to state with counter " + std::to_string(i));
        }
        vm_.step();
    }
}
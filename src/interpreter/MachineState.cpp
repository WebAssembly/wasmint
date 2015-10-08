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


#include "MachineState.h"
#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include <Function.h>
#include "Thread.h"

namespace wasmint {

    void MachineState::useModule(wasm_module::Module &module) {
        modules_[module.name()] = &module;

        for (wasm_module::Global &global : module.globals()) {
            globals_[global.name()] = wasm_module::Variable(global.type());
        }
    }

    Thread &MachineState::createThread() {
        Thread *newThread = new Thread(*this);
        threads_.push_back(newThread);
        return *threads_.back();
    }

    MachineState::~MachineState() {
        for (Thread *thread : threads_) {
            delete thread;
        }
    }

}
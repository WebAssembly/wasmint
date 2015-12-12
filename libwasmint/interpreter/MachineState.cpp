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
#include <Function.h>

namespace wasmint {

    void MachineState::useModule(wasm_module::Module &module, bool takeMemoryOwnership) {
        modules_[module.name()] = &module;

        if (takeMemoryOwnership) {
            modulesToDelete_.push_back(&module);
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
        for (wasm_module::Module* module : modulesToDelete_) {
            delete module;
        }
    }

    void MachineState::serialize(ByteOutputStream& stream) const {
        stream.writeUInt64(threads_.size());
        for (Thread* thread : threads_) {
            thread->serialize(stream);
        }
    }

    void MachineState::setState(ByteInputStream& stream) {
        uint64_t numberOfThreads = stream.getUInt64();
        for (uint64_t i = 0; i < numberOfThreads; i++) {
            Thread& thread = createThread();
            thread.setState(stream);
        }
    }

    const wasm_module::Instruction *MachineState::getInstruction(const wasm_module::InstructionAddress& address) {
        wasm_module::Module& module = getModule(address.moduleName());
        wasm_module::Function& function = module.getFunction(address.functionName());
        return function.instruction(address);
    }
}
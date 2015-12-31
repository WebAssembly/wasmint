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


#ifndef WASMINT_REGISTERMACHINE_H
#define WASMINT_REGISTERMACHINE_H

#include <interpreter/heap/Heap.h>
#include <Function.h>
#include <Module.h>
#include <iostream>
#include "ByteCode.h"
#include "JITCompiler.h"
#include "VMThread.h"
#include "CompiledFunction.h"
#include <stdexcept>

namespace wasmint {
    class VMState {

        Heap heap_;
        InstructionCounter instructionCounter_;
        VMThread thread_;

    public:
        void useModule(wasm_module::Module &module) {
            if (module.heapData().startSize() != 0) {
                if (heap_.size() == 0) {
                    heap_ = Heap(module.heapData());
                } else {
                    throw std::domain_error("Only one module with heap supported at the moment");
                }
            }
        }

        VMThread& startAtFunction(WasmintVM* vm, std::size_t index) {
            thread_ = VMThread(vm);
            thread_.enterFunction(index);
            return thread_;
        }

        bool step() {
            ++instructionCounter_;
            if (!thread_.finished()) {
                thread_.step(heap_);
                return true;
            }
            return false;
        }

        void stepUntilFinished() {
            while (!thread_.finished()) {
                ++instructionCounter_;
                thread_.step(heap_);
            }
        }

        Heap& heap() {
            return heap_;
        }

        VMThread& thread() {
            return thread_;
        }

        const InstructionCounter& instructionCounter() const {
            return instructionCounter_;
        }

        void instructionCounter(const InstructionCounter& counter) {
            this->instructionCounter_ = counter;
        }

        bool gotTrap() const {
            return thread_.gotTrap();
        }

        const std::string& trapReason() const {
            return thread_.trapReason();
        }
    };
};




#endif //WASMINT_REGISTERMACHINE_H

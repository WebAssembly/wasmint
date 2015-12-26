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

namespace wasmint {
    class RegisterMachine {

        Heap heap_;
        VMThread runner_;
        std::vector<CompiledFunction> functions_;
        std::vector<wasm_module::Module*> modulesToDelete_;

        void compileFunction(const wasm_module::Function* function) {
            CompiledFunction compiledFunction(function);
            functions_.push_back(compiledFunction);
        }

        VMThread thread_;

        void linkModules() {
            for (CompiledFunction& function : functions_) {
                function.jitCompiler().linkGlobally(this);
            }
        }

    public:
        RegisterMachine() : runner_(this) {
        }

        ~RegisterMachine() {
            for (wasm_module::Module* module : modulesToDelete_) {
                delete module;
            }
        }

        void loadModule(wasm_module::Module* module, bool takeMemoryOwnership) {
            if (module->heapData().startSize() != 0) {
                if (heap_.size() == 0) {
                    heap_ = Heap(module->heapData());
                } else {
                    throw std::domain_error("Only one module with heap supported at the moment");
                }
            }

            for (auto function :  module->functions()) {
                compileFunction(function);
            }

            if (takeMemoryOwnership) {
                modulesToDelete_.push_back(module);
            }
        }

        VMThread& startAtFunction(wasm_module::Function& function) {
            linkModules();
            for (std::size_t i = 0; i < functions_.size(); i++) {
                if (&functions_[i].function() == &function) {
                    thread_ = VMThread(this);
                    thread_.enterFunction(i);
                    return thread_;
                }
            }
        }

        bool step() {
            if (!thread_.finished()) {
                thread_.step(heap_);
                return true;
            }
            return false;
        }

        void stepUntilFinished() {
            while (!thread_.finished()) {
                thread_.step(heap_);
            }
        }

        const CompiledFunction& getCompiledFunction(std::size_t index) const {
            return functions_.at(index);
        }

        std::size_t getIndex(const CompiledFunction& function) {
            for (std::size_t i = 0; i < functions_.size(); i++) {
                if (&functions_[i] == &function) {
                    return i;
                }
            }
            throw std::domain_error("Function " + function.function().name() + " not compiled in this RegisterMachine");
        }

        uint32_t getNumberOfCompiledFunction() const {
            return (uint32_t) functions_.size();
        }

        Heap& heap() {
            return heap_;
        }

    };
};




#endif //WASMINT_REGISTERMACHINE_H

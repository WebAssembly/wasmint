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


#ifndef WASMINT_WASMINTVM_H
#define WASMINT_WASMINTVM_H

#include "VMState.h"
#include "History.h"

namespace wasmint {
    class WasmintVM {

        VMState state_;
        History history_;

        bool reconstructing_ = false;
        bool historyEnabled_ = true;

        std::vector<CompiledFunction> functions_;
        std::vector<wasm_module::Module*> modulesToDelete_;

        void linkModules() {
            for (CompiledFunction& function : functions_) {
                function.jitCompiler().linkGlobally(this);
            }
        }

        void compileFunction(const wasm_module::Function* function) {
            CompiledFunction compiledFunction(function);
            functions_.push_back(compiledFunction);
        }

    public:
        WasmintVM() {
        }
        virtual ~WasmintVM() {
            for (wasm_module::Module* module : modulesToDelete_) {
                delete module;
            }
        }

        void startAtFunction(const wasm_module::Function& function, bool enableHistory = true) {
            linkModules();
            for (std::size_t i = 0; i < functions_.size(); i++) {
                if (&functions_[i].function() == &function) {
                    state_.startAtFunction(this, i);
                    startHistoryRecording();
                    return;
                }
            }
            throw std::domain_error("Can't find compiled function with name " + function.name());
        }

        void useModule(wasm_module::Module &module, bool takeMemoryOwnership) {
            state_.useModule(module);
            for (auto function :  module.functions()) {
                compileFunction(function);
            }

            if (takeMemoryOwnership) {
                modulesToDelete_.push_back(&module);
            }
        }

        const CompiledFunction& getCompiledFunction(std::size_t index) const {
            return functions_.at(index);
        }

        std::size_t getIndex(const std::string& module, const std::string& functionName) {
            for (std::size_t i = 0; i < functions_.size(); i++) {
                if (functions_[i].function().name() == functionName && functions_[i].function().module().name() == module) {
                    return i;
                }
            }
            throw std::domain_error("Function " + functionName + " in module " + module + " not compiled in this VM");
        }

        uint32_t getNumberOfCompiledFunction() const {
            return (uint32_t) functions_.size();
        }

        void step() {
            state_.step();
        }

        void stepUntilFinished() {
            while (!state_.thread().finished()) {
                state_.step();
            }
        }

        void stepBack() {
            InstructionCounter targetCounter = state_.instructionCounter();
            history_.getLastCheckpoint().apply(state_);
            reconstructing_ = true;
            while (state_.instructionCounter() != targetCounter) {
                state_.step();
            }
            reconstructing_ = false;
        }

        bool gotTrap() const {
            return state_.gotTrap();
        }

        const std::string& trapReason() const {
            return state_.trapReason();
        }

        Heap& heap() {
            return state_.heap();
        }

        void startHistoryRecording() {
            history_.clear();
            history_.addCheckpoint(state_);
        }

        bool reconstructing() const {
            return reconstructing_;
        }

        History& history() {
            return history_;
        }

        const InstructionCounter& instructionCounter() const {
            return state_.instructionCounter();
        }
    };

}



#endif //WASMINT_WASMINTVM_H

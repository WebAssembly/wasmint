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

        std::vector<CompiledFunction> functions_;
        std::vector<wasm_module::Module*> modules_;
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

        void startAtFunction(const wasm_module::Function& function, bool enableHistory = true);

        void startAtFunction(const wasm_module::Function& function, const std::vector<wasm_module::Variable>& parameters, bool enableHistory = true);

        void loadModule(const std::string& path);

        void loadModuleFromData(const std::string &moduleContent);

        void loadModule(wasm_module::Module &module, bool takeMemoryOwnership) {
            if (modules_.empty())
                state_.useModule(module);

            modules_.push_back(&module);
            for (auto function :  module.functions()) {
                compileFunction(function);
            }

            if (takeMemoryOwnership) {
                modulesToDelete_.push_back(&module);
            }
        }

        const std::vector<wasm_module::Module*> modules() const {
            return modules_;
        }

        CompiledFunction& getCompiledFunction(std::size_t index) {
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
            history_.latestStateCounter(state_.instructionCounter());
        }

        void stepUntilFinished(bool stopAtBreakpoints = false) {
            state_.stepUntilFinished(stopAtBreakpoints);
            history_.latestStateCounter(state_.instructionCounter());
        }

        void stepBack() {
            InstructionCounter targetCounter = state_.instructionCounter();
            --targetCounter;
            history_.setToState(targetCounter, state_);
        }

        void simulateTo(InstructionCounter targetCounter) {
            history_.setToState(targetCounter, state_);
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
            return history_.reconstructing();
        }

        History& history() {
            return history_;
        }

        const InstructionCounter& instructionCounter() const {
            return state_.instructionCounter();
        }

        const VMState& state() const {
            return state_;
        }

        VMState& state() {
            return state_;
        }
        bool finished() const {
            return state().thread().finished();
        }

        void addBreakpoint(const wasm_module::Instruction* instruction, BreakpointHandler* handler = nullptr) {
            for (CompiledFunction& function : functions_) {
                function.addBreakpoint(instruction, handler);
            }
        }

    };

}



#endif //WASMINT_WASMINTVM_H

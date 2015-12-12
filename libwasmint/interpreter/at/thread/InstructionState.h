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

#ifndef WASMINT_INSTRUCTIONSTATE_H
#define WASMINT_INSTRUCTIONSTATE_H

#include <vector>
#include <Variable.h>
#include <interpreter/StepResult.h>
#include <stdexcept>
#include <ExceptionWithMessage.h>
#include <serialization/ByteInputStream.h>
#include <interpreter/at/InstructionExecutor.h>
#include <serialization/ByteOutputStream.h>

namespace wasm_module {
    class Instruction;
}

namespace wasmint {

    ExceptionMessage(IncompatibleChildReturnType)
    ExceptionMessage(StateHasNoBranchValue)

    class InterpreterThread;
    class MachineState;

    class InstructionState {

        uint32_t state_ = 0;
        bool finished_ = false;
        bool unhandledSignal_ = false;

        std::vector<wasm_module::Variable> results_;
        const wasm_module::Instruction* instruction_ = nullptr;

        wasm_module::Variable branchValue_;
        bool hasBranchValue_ = false;
        InterpreterThread * thread_ = nullptr;

        void finishSignal(StepResult result);

    public:
        InstructionState() {
        }
        InstructionState(InterpreterThread & thread_, const wasm_module::Instruction& instruction);

        virtual ~InstructionState();

        void step();

        bool finished() const {
            return finished_;
        }

        const wasm_module::Instruction& instruction() {
            return *instruction_;
        }

        bool unhandledSignal() const {
            return unhandledSignal_;
        }

        uint32_t state() const {
            return state_;
        }

        std::vector<wasm_module::Variable> &results() {
            return results_;
        }

        void state(uint32_t newState) {
            state_ = newState;
        }

        void clearResults() {
            results_.clear();
        }

        void setThread(InterpreterThread & newThread) {
            thread_ = &newThread;
        }

        const wasm_module::Variable& branchValue() const {
            if (!hasBranchValue_)
                throw StateHasNoBranchValue("branchValue() was called on InstructionState instance without a branchValue");
            return branchValue_;
        }

        void branchValue(const wasm_module::Variable& newValue) {
            branchValue_ = newValue;
            hasBranchValue_ = true;
        }

        bool hasBranchValue() const {
            return hasBranchValue_;
        }

        virtual void serialize(ByteOutputStream& stream) const;

        void setState(ByteInputStream& stream, MachineState& state);
    };

}

#endif //WASMINT_INSTRUCTIONSTATE_H

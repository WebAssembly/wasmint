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
#include <serialization/Serializeable.h>
#include <serialization/ByteInputStream.h>

namespace wasm_module {
    class Instruction;
}

namespace wasmint {

    ExceptionMessage(IncompatibleChildReturnType)
    ExceptionMessage(StateHasNoBranchValue)

    class Thread;
    class MachineState;

    class InstructionState : public Serializeable {

        uint32_t state_ = 0;
        wasm_module::Variable result_;
        bool finished_ = false;
        std::vector<wasm_module::Variable> results_;
        const wasm_module::Instruction* instruction_ = nullptr;

        InstructionState* parent_ = nullptr;

        wasm_module::Variable branchValue_;
        bool hasBranchValue_ = false;
        InstructionState* childInstruction = nullptr;
        Thread* thread_ = nullptr;

    public:
        InstructionState(Thread& thread_, wasm_module::Instruction *instruction = nullptr, InstructionState* parent = nullptr);

        virtual ~InstructionState();

        wasm_module::Variable result() {
            return result_;
        }

        StepResult step(Thread &thread);

        bool finished() {
            return finished_;
        }

        const wasm_module::Instruction* instruction() {
            return instruction_;
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

        InstructionState &getChildOrThis() {
            if (childInstruction != nullptr)
                return childInstruction->getChildOrThis();
            return *this;
        }

        void clearResults() {
            results_.clear();
        }

        InstructionState* parent() {
            if (parent_ == nullptr)
                throw std::domain_error("InstructionState has no parent. parent() can't be executed");
            return parent_;
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

        virtual void serialize(ByteOutputStream& stream) const override;

        void setState(ByteInputStream& stream, MachineState& state);
    };

}

#endif //WASMINT_INSTRUCTIONSTATE_H

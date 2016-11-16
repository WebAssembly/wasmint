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


#ifndef WASMINT_FUNCTIONFRAME_H
#define WASMINT_FUNCTIONFRAME_H

#include <cstdint>
#include <vector>
#include <interpreter/heap/Heap.h>
#include "ByteCode.h"
#include "CompiledFunction.h"
#include "ValueStack.h"

namespace wasmint {
    class VMThread;

    class FunctionFrame {
        const ByteCode* code_ = nullptr;
        std::vector<uint64_t> variables_;

        uint16_t functionTargetRegister_ = 0;

        uint32_t instructionPointer_ = 0;
        CompiledFunction* function_ = nullptr;

        ValueStack stack_;

        void dumpStatus(ByteOpcodes::Values opcode, uint16_t opcodeData);

        void stepInternal(VMThread &runner, Heap &heap);


    public:
        FunctionFrame() {
        }

        FunctionFrame(CompiledFunction& function) : function_(&function) {
            code_ = &function.code();
            uint16_t numberOfRegisters = popFromCode<uint16_t>();

            uint16_t numberOfVariables = popFromCode<uint16_t>();
            variables_.resize(numberOfVariables, 0);
        }

        void passFunctionResult(uint64_t value) {
            stack_.push(value);
        }

        void passFunctionResult(const wasm_module::Variable& value) {
            if (&value.type() != wasm_module::Void::instance())
                stack_.push(value.primitiveValue());
        }
        template<typename T>
        T popFromCode() {
            T result = code_->get<T>(instructionPointer_);
            instructionPointer_ += sizeof(T);
            return result;
        }

        template<typename T>
        void popFromCode(T* target) {
            code_->getUnsafe<T>(target, instructionPointer_);
            instructionPointer_ += sizeof(T);
        }

        template<typename T>
        T peekFromCode(uint32_t offset = 0) {
            T result = code_->get<T>(offset + instructionPointer_);
            instructionPointer_ += sizeof(T);
            return result;
        }

        uint64_t getVariable(uint16_t index) {
            uint64_t result = *(variables_.data() + index);
            return result;
        }

        void setVariable(uint64_t index, uint64_t value) {
            *(variables_.data() + index) = value;
        }

        void step(VMThread &runner, Heap &heap);
        bool stepDebug(VMThread &runner, Heap &heap);

        bool operator==(const FunctionFrame& other) const {
            if (code_ != other.code_)
                return false;

            if (variables_.size() != other.variables_.size())
                return false;

            if (stack_ != other.stack_)
                return false;

            for (std::size_t i = 0; i < variables_.size(); i++) {
                if (variables_[i] != other.variables_[i])
                    return false;
            }

            return functionTargetRegister_ == other.functionTargetRegister_
                    && instructionPointer_ == other.instructionPointer_
                    && function_ == other.function_;
        }

        bool operator!=(const FunctionFrame& other) const {
            return !(*this == other);
        }

        const CompiledFunction& function() const {
            return *function_;
        }

        template<typename T>
        void push(T value) {
            stack_.push<T>(value);
        }

        template<typename T>
        T peek() {
            return stack_.peek<T>();
        }

        template<typename T>
        T pop() {
            return stack_.pop<T>();
        }
    };
}



#endif //WASMINT_FUNCTIONFRAME_H

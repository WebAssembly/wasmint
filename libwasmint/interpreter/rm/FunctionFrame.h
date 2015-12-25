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

namespace wasmint {
    class ByteCodeRunner;

    class FunctionFrame {
        std::vector<uint64_t> registers_;
        std::vector<uint64_t> variables_;

        uint16_t functionTargetRegister_ = 0;

        std::size_t instructionPointer_ = 0;
        CompiledFunction* function_;

    public:
        FunctionFrame() {
        }

        FunctionFrame(CompiledFunction& function) : function_(&function) {

            uint16_t numberOfRegisters;
            function_->code().get<uint16_t>(&numberOfRegisters, 0);
            registers_.resize(numberOfRegisters);
            instructionPointer_ = sizeof numberOfRegisters;

            uint16_t numberOfVariables;
            function_->code().get<uint16_t>(&numberOfVariables, 0);
            registers_.resize(numberOfVariables);
            instructionPointer_ += sizeof numberOfVariables;
        }

        void passFunctionResult(uint64_t value) {
            setRegister(functionTargetRegister_, value);
        }

        template<typename T>
        T popFromCode() {
            T result = function_->code().get<uint16_t>(instructionPointer_);
            instructionPointer_ += sizeof(T);
            return result;
        }

        template<typename T>
        T peekFromCode(uint32_t offset = 0) {
            T result = function_->code().get<uint16_t>(offset + instructionPointer_);
            instructionPointer_ += sizeof(T);
            return result;
        }

        template<typename T>
        void setRegister(uint16_t index, T value) {
            std::memcpy(registers_.data() + index, &value,sizeof(T));
        }

        template<typename T>
        T getRegister(unsigned index) {
            T result;
            std::memcpy(&result, registers_.data() + index, sizeof(T));
            return result;
        }

        uint64_t getVariable(uint16_t index) {
            uint64_t result;
            std::memcpy(&result, registers_.data() + index, sizeof(uint64_t));
            return result;
        }

        void setVariable(uint64_t index, uint64_t value) {
            std::memcpy(variables_.data() + index, &value, sizeof(uint64_t));
        }

        void step(ByteCodeRunner& runner, Heap& heap);

    };
}



#endif //WASMINT_FUNCTIONFRAME_H

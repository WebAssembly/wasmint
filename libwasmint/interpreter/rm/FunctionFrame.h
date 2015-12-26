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
    class VMThread;

    class FunctionFrame {
        std::vector<uint64_t> registers_;
        std::vector<uint64_t> variables_;

        uint16_t functionTargetRegister_ = 0;

        uint32_t instructionPointer_ = 0;
        const CompiledFunction* function_ = nullptr;

        void dumpStatus(ByteOpcodes::Values opcode, uint16_t opcodeData);

        void stepInternal(VMThread &runner, Heap &heap);

    public:
        FunctionFrame() {
        }

        FunctionFrame(const CompiledFunction& function) : function_(&function) {
            uint16_t numberOfRegisters = popFromCode<uint16_t>();
            registers_.resize(numberOfRegisters, 0);

            uint16_t numberOfVariables = popFromCode<uint16_t>();
            variables_.resize(numberOfVariables, 0);
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
            std::memcpy(&result, variables_.data() + index, sizeof(uint64_t));
            return result;
        }

        void setVariable(uint64_t index, uint64_t value) {
            if (index >= variables_.size())
                throw std::domain_error("Can't access variable with index " + std::to_string(index)
                                        + ". Current function has variable array of size " + std::to_string(variables_.size()));
            std::memcpy(variables_.data() + index, &value, sizeof(uint64_t));
        }

        void step(VMThread &runner, Heap &heap);

    };
}



#endif //WASMINT_FUNCTIONFRAME_H

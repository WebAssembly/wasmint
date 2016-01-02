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


#include <cmath>
#include <limits>
#include <instructions/Instructions.h>
#include "VMThread.h"
#include "WasmintVM.h"

namespace wasmint {


    void VMThread::enterFunction(std::size_t functionId) {
        pushFrame(FunctionFrame(machine().getCompiledFunction(functionId)));
    }

    void VMThread::enterFunction(std::size_t functionId, uint32_t parameterSize, uint16_t parameterRegisterOffset) {
        CompiledFunction& targetFunction = machine().getCompiledFunction(functionId);

        if (targetFunction.function().mainInstruction()->id() == InstructionId::NativeInstruction) {
            auto nativeInstruction = dynamic_cast<const wasm_module::NativeInstruction*>(targetFunction.function().mainInstruction());

            if (machine().reconstructing()) {
                if (nativeInstruction->returnType() != wasm_module::Void::instance()) {
                    currentFrame_->passFunctionResult(machine().history().getNativeFunctionReturnValue(machine().instructionCounter()));
                }
            } else {
                std::vector<wasm_module::Variable> parameters;
                parameters.reserve(nativeInstruction->childrenTypes().size());
                uint16_t i = 0;
                for (const wasm_module::Type* type : nativeInstruction->childrenTypes()) {
                    wasm_module::Variable parameter(type);
                    uint64_t value = currentFrame_->getRegister<uint64_t>(parameterRegisterOffset + i);
                    memcpy(parameter.value(), &value, type->size());
                    i++;
                }

                wasm_module::Variable result = nativeInstruction->call(parameters);

                if (nativeInstruction->returnType() != wasm_module::Void::instance()) {
                    machine().history().addNativeFunctionReturnValue(machine().instructionCounter(),
                                                                     result.primitiveValue());
                }

                currentFrame_->passFunctionResult(result);
            }
        } else {
            pushFrame(FunctionFrame(targetFunction));

            for (uint32_t i = 0; i < parameterSize; i++) {
                currentFrame().setVariable(i, frames_.at(frames_.size() - 2).getRegister<uint64_t>(parameterRegisterOffset + i));
            }
        }
    }

    void VMThread::finishFrame(uint64_t result) {
        if (frames_.empty())
            throw std::domain_error("Can't call finishFrame(): frame stack is empty!");

        machine().history().threadStackShrinked(*this);

        frames_.resize(frames_.size() - 1);
        if (!frames_.empty()) {
            currentFrame_ = &frames_.back();
            currentFrame_->passFunctionResult(result);
        }
        else
            finished_ = true;
    }
}
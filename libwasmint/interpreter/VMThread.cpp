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
        std::vector<wasm_module::Variable> emptyParameters;
        enterFunction(functionId, emptyParameters);
    }

    void VMThread::enterFunction(std::size_t functionId, const std::vector<wasm_module::Variable>& parameters) {
        frames_.clear();
        CompiledFunction& function = machine().getCompiledFunction(functionId);
        if (function.function().parameters().size() != parameters.size()) {
            throw InvalidCallParameters("Function " + function.function().name() + " takes " +
                                        std::to_string(function.function().parameters().size())
                                        + " parameters, but " + std::to_string(parameters.size()) + " were given");
        }
        for (std::size_t i = 0; i < parameters.size(); i++) {
            if (&parameters[i].type() != function.function().parameters()[i]) {
                throw InvalidCallParameters("Type mismatch: Parameter " + std::to_string(i + 1) + " needs type "
                                            + function.function().parameters()[i]->name() + " but " + parameters[i].type().name()
                                            + " was given");
            }
        }
        pushFrame(FunctionFrame(machine().getCompiledFunction(functionId)));
        for (uint64_t i = 0; i < parameters.size(); i++) {
            frames_.front().setVariable(i, parameters[i].primitiveValue());
        }
    }

    void VMThread::enterFunction(std::size_t functionId, uint32_t parameterSize, uint16_t parameterRegisterOffset) {
        CompiledFunction& targetFunction = machine().getCompiledFunction(functionId);
        const wasm_module::Function& function = targetFunction.function();

        if (function.isNative()) {
            auto nativeInstruction = static_cast<const wasm_module::NativeInstruction*>(function.mainInstruction());

            if (machine().reconstructing()) {
                if (function.variadic()) {
                    for (uint16_t i = 0; i < parameterSize; i++) {
                        frames_.back().popFromCode<uint8_t>();
                    }
                }
                if (nativeInstruction->returnType() != wasm_module::Void::instance()) {
                    currentFrame_->passFunctionResult(machine().history().getNativeFunctionReturnValue(machine().instructionCounter()));
                }
            } else {

                if (!function.deterministic() && machine().history().enabled()) {
                    machine().history().getLastCheckpoint().influencedByExternalState(true);
                }

                std::vector<wasm_module::Variable> parameters;
                parameters.reserve(nativeInstruction->childrenTypes().size());
                for (uint16_t i = 0; i < parameterSize; i++) {
                    const wasm_module::Type* type = nullptr;
                    if (function.variadic()) {
                        uint8_t typeId = frames_.back().popFromCode<uint8_t>();
                        switch(typeId) {
                            case 0:
                                type = wasm_module::Int32::instance();
                                break;
                            case 1:
                                type = wasm_module::Int64::instance();
                                break;
                            case 2:
                                type = wasm_module::Float32::instance();
                                break;
                            case 3:
                                type = wasm_module::Float64::instance();
                                break;
                            default:
                                assert(false);
                        }
                    } else {
                        type = function.parameters().at(i);
                    }
                    wasm_module::Variable parameter(type);
                    uint64_t value = currentFrame_->getRegister<uint64_t>(parameterRegisterOffset + i);
                    memcpy(parameter.value(), &value, type->size());
                    parameters.push_back(parameter);
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

        if (frames_.size() == 1) {
            if (frames_.back().function().function().returnType() != wasm_module::Void::instance()) {
                result_ = wasm_module::Variable(frames_.back().function().function().returnType());
                result_.setFromPrimitiveValue(result);
            }
        }

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
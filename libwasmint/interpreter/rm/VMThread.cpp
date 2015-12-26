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
#include "VMThread.h"
#include "RegisterMachine.h"

namespace wasmint {


    void VMThread::enterFunction(std::size_t functionId) {
        pushFrame(FunctionFrame(machine().getCompiledFunction(functionId)));
    }

    void VMThread::enterFunction(std::size_t functionId, uint32_t parameterSize, uint16_t parameterRegisterOffset) {
        pushFrame(FunctionFrame(machine().getCompiledFunction(functionId)));

        for (uint32_t i = 0; i < parameterSize; i++) {
            currentFrame().setVariable(i, frames_.at(frames_.size() - 2).getRegister<uint64_t>(parameterRegisterOffset + i));
        }
    }
}
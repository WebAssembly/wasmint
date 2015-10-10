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



#ifndef WASMINT_STEPRESULT_H
#define WASMINT_STEPRESULT_H

#include <Variable.h>

namespace wasm_module {
    class Instruction;
}

namespace wasmint {

    enum class Signal {
        None,
        Break,
        Continue,
        Return,
        AssertTrap,
    };

    class StepResult {
        wasm_module::Instruction *newChildInstruction_ = nullptr;
        wasm_module::Variable result_;
        Signal signal_ = Signal::None;

    public:
        StepResult(wasm_module::Instruction *newChildInstruction) : newChildInstruction_(newChildInstruction) {
        }

        StepResult(const wasm_module::Variable &result) : result_(result) {
        }

        StepResult(Signal signal, wasm_module::Variable result = wasm_module::Variable())
                : signal_(signal), result_(result) {
        }

        StepResult() {

        }

        wasm_module::Variable result() {
            return result_;
        }

        wasm_module::Instruction *newChildInstruction() {
            return newChildInstruction_;
        }

        Signal signal() {
            return signal_;
        }
    };

}

#endif //WASMINT_STEPRESULT_H

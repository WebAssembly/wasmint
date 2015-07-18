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

#ifndef WASMINT_INSTRUCTIONEXECUTOR_H
#define WASMINT_INSTRUCTIONEXECUTOR_H


#include <interpreter/StepResult.h>
#include "Thread.h"

ExceptionMessage(UnknownInstruction)

class InstructionExecutor {

public:
    static StepResult execute(Instruction& instruction, Thread& thread);
    static bool handleSignal(Instruction& instruction, InstructionState& currentState, Signal signal);
};


#endif //WASMINT_INSTRUCTIONEXECUTOR_H

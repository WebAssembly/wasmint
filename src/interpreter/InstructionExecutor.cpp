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

#include "InstructionExecutor.h"
#include <instructions/controlflow/Block.h>
#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include <instructions/controlflow/DoWhile.h>
#include <instructions/controlflow/Forever.h>
#include <instructions/controlflow/If.h>
#include <instructions/controlflow/Return.h>

#include <instructions/heap/Int32Load.h>
#include <instructions/heap/Int32Store.h>

#include <instructions/I32/I32Add.h>
#include <instructions/I32/I32Div.h>
#include <instructions/I32/I32Mul.h>
#include <instructions/I32/I32Sub.h>

#include <instructions/FunctionCall.h>
#include <instructions/GetGlobal.h>
#include <instructions/GetLocal.h>
#include <instructions/Literal.h>
#include <instructions/Print.h>
#include <instructions/SetGlobal.h>
#include <instructions/SetLocal.h>

#include "MachineState.h"

StepResult InstructionExecutor::execute(Instruction& instruction, Thread& thread) {
    InstructionState& state = thread.getInstructionState();

    /**
     * Control flow
     */
    if (typeid(instruction) == typeid(Block)) {
        if (state.state() < instruction.children().size()) {
            return StepResult(instruction.children().at(state.state()));
        } else {
            return StepResult();
        }
    } else if (typeid(instruction) == typeid(Break)) {
        return StepResult(Signal::Break);
    } else if (typeid(instruction) == typeid(Continue)) {
        return StepResult(Signal::Continue);
    } else if (typeid(instruction) == typeid(DoWhile)) {
        switch(state.state()) {
            case 0:
            case 1:
                return instruction.children().at(0);
            case 2:
                return instruction.children().at(1);
            default:
                if (Int32::getValue(state.results().back()) != 0) {
                    state.state(0);
                } else {
                    return StepResult();
                }
        }
    } else if (typeid(instruction) == typeid(Forever)) {
        if (thread.getInstructionState().state() >= 10) {
            return StepResult();
        }
        thread.getInstructionState().clearResults();
        thread.getInstructionState().state(0);
        return StepResult(instruction.children().front());
    } else if (typeid(instruction) == typeid(If)) {
        switch(state.state()) {
            case 0:
                return instruction.children().at(0);
            case 1:
                if (Int32::getValue(state.results().front()) != 0) {
                    return instruction.children().at(1);
                }
            default:
                return StepResult();

        }
    } else if (typeid(instruction) == typeid(Return)) {
        switch(state.state()) {
            case 0:
                return instruction.children().at(0);
            default:
                return state.results().front();

        }
    }
    /**
     * heap access
     */
    else if (typeid(instruction) == typeid(Int32Load)) {
        switch(state.state()) {
            case 0:
                return StepResult(instruction.children().at(0));
            default:
                uint32_t offset = static_cast<uint32_t>(Int32::getValue(state.results().back()));

                std::vector<uint8_t> bytes = thread.runtimeEnvironment().heap().getBytes(offset, Int32::instance()->size());

                Variable result = Variable(Int32::instance()->localType());
                result.setValue(bytes);
                return result;
        }
    } else if (typeid(instruction) == typeid(Int32Store)) {
        switch(state.state()) {
            case 0:
                return StepResult(instruction.children().at(0));
            case 1:
                return StepResult(instruction.children().at(1));
            default:

                uint32_t offset = static_cast<uint32_t>(Int32::getValue(state.results().at(0)));

                Variable value = state.results().at(1);
                thread.runtimeEnvironment().heap().setBytes(offset, value.data());
                return StepResult(value);
        }
    }
    /**
     * i32 instructions
     */
    else if (typeid(instruction) == typeid(I32Add)) {
        switch(state.state()) {
            case 0:
                return StepResult(instruction.children().at(0));
            case 1:
                return StepResult(instruction.children().at(1));
            default:
                int32_t left = Int32::getValue(state.results().at(0));
                int32_t right = Int32::getValue(state.results().at(1));

                Variable result = Variable(Int32::instance());
                Int32::setValue(result, left + right);
                return result;
        }
    } else if (typeid(instruction) == typeid(I32Div)) {
        switch(state.state()) {
            case 0:
                return StepResult(instruction.children().at(0));
            case 1:
                return StepResult(instruction.children().at(1));
            default:
                int32_t left = Int32::getValue(state.results().at(0));
                int32_t right = Int32::getValue(state.results().at(1));

                if (right == 0)
                    throw DivisionThroughZero();

                Variable result = Variable(Int32::instance());
                Int32::setValue(result, left + right);
                return result;
        }
    } else if (typeid(instruction) == typeid(I32Mul)) {
        switch(state.state()) {
            case 0:
                return StepResult(instruction.children().at(0));
            case 1:
                return StepResult(instruction.children().at(1));
            default:
                int32_t left = Int32::getValue(state.results().at(0));
                int32_t right = Int32::getValue(state.results().at(1));

                Variable result = Variable(Int32::instance());
                Int32::setValue(result, left * right);
                return result;
        }
    } else if (typeid(instruction) == typeid(I32Sub)) {
        switch(state.state()) {
            case 0:
                return StepResult(instruction.children().at(0));
            case 1:
                return StepResult(instruction.children().at(1));
            default:
                int32_t left = Int32::getValue(state.results().at(0));
                int32_t right = Int32::getValue(state.results().at(1));

                Variable result = Variable(Int32::instance());
                Int32::setValue(result, left - right);
                return result;
        }
    }
    /**
     * other instructions
     */
    else if (typeid(instruction) == typeid(FunctionCall)) {
        if (state.state() < instruction.children().size()) {
            return StepResult(instruction.children().at(0));
        } else if (state.state() == instruction.children().size()) {
            std::vector<Variable> parameters;
            for(uint32_t i = 0; i < parameters.size(); i++) {
                parameters[i] = state.results().at(i);
            }
            return StepResult(thread.callFunction(dynamic_cast<FunctionCall&>(instruction).functionSignature.name()));
        } else {
            return state.results().back();
        }
    } else if (typeid(instruction) == typeid(GetGlobal)) {
        return thread.runtimeEnvironment().global(dynamic_cast<GetGlobal&>(instruction).globalName);
    } else if (typeid(instruction) == typeid(GetLocal)) {
        return thread.variable(dynamic_cast<GetLocal&>(instruction).localIndex);
    } else if (typeid(instruction) == typeid(Literal)) {
        return StepResult(dynamic_cast<Literal&>(instruction).literalValue);
    } else if (typeid(instruction) == typeid(Print)) {
        switch(state.state()) {
            case 0:
                return instruction.children().at(0);
            default:
                thread.runtimeEnvironment().print(std::to_string(Int32::getValue(state.results().at(0))));
                return StepResult();

        }
    } else if (typeid(instruction) == typeid(SetGlobal)) {
        switch(state.state()) {
            case 0:
                return instruction.children().at(0);
            default:
                Variable result = thread.runtimeEnvironment().global(dynamic_cast<SetGlobal&>(instruction).globalName) = state.results().at(0);
                return result;

        }
    } else if (typeid(instruction) == typeid(SetLocal)) {
        switch(state.state()) {
            case 0:
                return instruction.children().at(0);
            default:
                Variable result = thread.variable(dynamic_cast<SetLocal&>(instruction).localIndex) = state.results().at(0);
                return result;

        }
    }

    else {
        throw UnknownInstruction(std::string("Pointer points to an unkown Instruction type"));
    }
}

bool InstructionExecutor::handleSignal(Instruction& instruction, InstructionState& currentState, Signal signal) {
    if (typeid(instruction) == typeid(Forever) || typeid(instruction) == typeid(DoWhile)) {
        if (signal == Signal::Break) {
            currentState.state(10);
            return true;
        }
        return signal == Signal::Continue;
    }

    return false;
}
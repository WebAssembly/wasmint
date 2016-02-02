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



#include "Function.h"
#include <instructions/Instruction.h>
#include <instructions/InstructionAddress.h>
#include <instructions/Instructions.h>

namespace wasm_module {

    Function::Function(FunctionContext &context, Instruction *mainInstruction)
            : FunctionContext(context), mainInstruction_(mainInstruction) {
        mainInstruction->function(this);
        mainInstruction->foreachChild([&](Instruction* child) {
            child->function(this);
        });
    }

    Function::~Function() {
        delete mainInstruction_;
    }

    Instruction* Function::instruction(const InstructionAddress& address) const {
        if (address.functionName() != name()) {
            throw std::domain_error("Given address describes instruction in function " + address.functionName() + " but current function name is " + name());
        }
        Instruction* result = mainInstruction_;
        for(std::size_t i = address.childrenIndized().size(); i > 0; i--) {
            result = result->children().at(address.childrenIndized()[i - 1]);
        }
        return result;
    }

    bool Function::isNative() const {
        return mainInstruction_->id() == InstructionId::NativeInstruction;
    }
}
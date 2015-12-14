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

#ifndef WASMINT_OPCODE_H
#define WASMINT_OPCODE_H


#include <cstdint>
#include <vector>
#include <ExceptionWithMessage.h>
#include <functional>
#include <branching/BranchInformation.h>

#include "types/Type.h"
#include "Variable.h"
#include "InstructionId.h"
#include "InstructionAddress.h"

namespace wasm_module {

    class ModuleContext;
    class FunctionContext;
    class Function;

    ExceptionMessage(IncompatibleChildReturnType)
    ExceptionMessage(IncompatibleNumberOfChildren)
    ExceptionMessage(InstructionHasNoParent)

    class InstructionState;
    class InstructionAddress;

    class Instruction {

        std::vector<Instruction *> children_;
        const Instruction* parent_ = nullptr;
        const Function* function_ = nullptr;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {

        }

    public:
        virtual ~Instruction() {
            for (Instruction* child : children()) {
                delete child;
            }
        }

        virtual void children(const std::vector<Instruction*>& newChildren) {
            for(Instruction* instruction : children_) {
                delete instruction;
            }
            children_ = newChildren;
            for (Instruction* instruction : children_) {
                instruction->parent(this);
            }
        }

        const Instruction& getNthParent(std::size_t n) const {
            const Instruction* result = this;
            for (std::size_t i = 0; i < n; i++) {
                result = result->parent();
            }
            return *result;
        }

        void triggerSecondStepEvaluate(ModuleContext& context, FunctionContext& functionContext);

        void foreachChild(const std::function<void(Instruction* instruction)>& lambda) {
            for (Instruction* child : children()) {
                child->foreachChild(lambda);
            }
            lambda(this);
        }

        void foreachChild(const std::function<void(const Instruction* instruction)>& lambda) const {
            for (const Instruction* child : children()) {
                child->foreachChild(lambda);
            }
            lambda(this);
        }

        void foreachParent(const std::function<bool(const Instruction* instruction)>& lambda) const {
            if (hasParent()) {
                if (lambda(parent())) {
                    parent()->foreachParent(lambda);
                }
            }
        }

        std::size_t getChildIndex(const Instruction* instruction) const;

        const std::vector<Instruction *>& children() const {
            return children_;
        }

        virtual bool typeCheckChildren() const {
            return true;
        }

        virtual const std::string& name() const = 0;

        virtual const std::vector<const Type *>& childrenTypes() const = 0;

        virtual const Type* returnType() const = 0;

        virtual InstructionId::Value id() const = 0;

        virtual std::string dataString() const {
            return name();
        }

        virtual std::string toSExprString() const {
            std::string result = "(";

            result += dataString();
            if (!children_.empty())
                result += " ";

            for (std::size_t i = 0; i < children_.size(); i++) {
                result += children_[i]->toSExprString();
                if (i != children_.size() - 1)
                    result += " ";
            }

            result += ")";

            return result;
        }

        virtual uint32_t labelCount() const {
            return 0;
        }

        virtual bool hasLabelName(const std::string& str) const {
            return false;
        }

        virtual uint32_t labelIndex(const std::string& str) const {
            return 0;
        }

        const Instruction* parent() const {
            if (!hasParent())
                throw InstructionHasNoParent(this->toSExprString());
            return parent_;
        }

        void parent(const Instruction* newParent) {
            parent_ = newParent;
        }

        bool hasParent() const {
            return parent_ != nullptr;
        }

        InstructionAddress getAddress() const;

        const Function* function() const {
            return function_;
        }

        void function(const Function* function) {
            function_ = function;
        }

        virtual const BranchInformation* branchInformation() const {
            return nullptr;
        }
    };
}

#endif //WASMINT_OPCODE_H

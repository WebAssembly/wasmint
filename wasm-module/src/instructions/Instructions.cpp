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

#include <sexpr_parsing/Types.h>
#include <branching/BranchTypeValidator.h>
#include "Instructions.h"
#include "UnreachableValidator.h"


namespace wasm_module {

    Literal::Literal(const sexpr::SExpr& expr) {
        std::string value = expr[1].value();
        std::string typeName = expr[0].value().substr(0, 3);

        literalValueStr_ = value;

        const Type* type = sexpr::Types::getByName(typeName);
        literalValue_ = Variable(type);
        type->parse(value, literalValue_.value());
    }

    LoadStoreInstruction::LoadStoreInstruction(const wasm_module::sexpr::SExpr& expr) {
        for (const sexpr::SExpr& child : expr.children()) {
            if (child.hasValue()) {
                if(child.value().find("offset=") == 0) {
                    std::string value = child.value().substr(std::string("offset=").size());
                    offset_ = (uint32_t) std::atol(value.c_str());
                }
            }
        }
    }

    void Loop::secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {
        bool hasChildReturnType = false;
        if (!children().empty()) {
            returnType_ = children().back()->returnType();
            hasChildReturnType = !UnreachableValidator::willNeverEvaluate(children().back());
        }
        const Type* branchType = BranchTypeValidator::checkBranchType(*this, 1);
        if (branchType) {
            if (hasChildReturnType) {
                if (returnType_ != branchType) {
                    returnType_ = Void::instance();
                }
            } else {
                returnType_ = branchType;
            }
        }
    }

    void Block::secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {
        if (!children().empty()) {
            returnType_ = children().back()->returnType();
        }
        const Type* branchType = BranchTypeValidator::checkBranchType(*this);
        if (branchType) {
            if (returnType_ != branchType) {
                returnType_ = Void::instance();
            }
        }
    }

    void BranchIf::secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {
        if (!labelName_.empty()) {
            branchInformation_ = BranchInformation::getBranchInformation(*this, labelName_, children().at(1)->returnType());
        } else {
            branchInformation_ = BranchInformation::getBranchInformation(*this, branchLabel_, children().at(1)->returnType());
        }
    }

    void Branch::secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {
        if (!labelName_.empty()) {
            branchInformation_ = BranchInformation::getBranchInformation(*this, labelName_, children().at(0)->returnType());
        } else {
            branchInformation_ = BranchInformation::getBranchInformation(*this, branchLabel_, children().at(0)->returnType());
        }
    }

    TableSwitch::TableSwitch(const sexpr::SExpr& expr, std::set<std::size_t>& subExprsToIgnore) {
        std::size_t index = 0;
        const sexpr::SExpr* tableExpr = nullptr;
        bool foundDefaultTarget = false;

        for (const sexpr::SExpr& subExpr : expr.children()) {
            if (subExpr.hasChildren()) {
                if (subExpr[0].value() == "table") {
                    if (tableExpr != nullptr)
                        throw MultipleTableExprsInTableSwitch(expr.toString());
                    tableExpr = &subExpr;
                    subExprsToIgnore.insert(index);
                } else {
                    if (tableExpr != nullptr && !foundDefaultTarget) {
                        defaultTarget_ = TableSwitchTarget::parse(subExpr);
                        subExprsToIgnore.insert(index);
                        foundDefaultTarget = true;
                    }
                }
            }
            index++;
        }
        if (tableExpr == nullptr)
            throw NoTableExprInTableSwitch(expr.toString());


        for (std::size_t i = 1; i < tableExpr->children().size(); i++) {
            TableSwitchTarget target = TableSwitchTarget::parse(tableExpr->children().at(i));
            targets_.push_back(target);
        }

        childrenTypes_.push_back(Int32::instance());

        index = 0;
        for (const sexpr::SExpr& subExpr : expr.children()) {
            if (subExpr.hasChildren()) {
                if (subExpr[0].value() != "table") {
                    childrenTypes_.push_back(Void::instance());
                }
            }
            index++;
        }
        childrenTypes_.resize(childrenTypes_.size() - 2);

        if (expr[1].hasValue()) {
            labelName_ = expr[1].value();
        }
    }

    bool TableSwitchTarget::isCase() const {
        return isCase_;
    }

    bool TableSwitchTarget::isBranch() const {
        return !isCase_;
    }

    const std::string &TableSwitchTarget::targetName() const {
        return targetName_;
    }

    TableSwitchTarget TableSwitchTarget::parse(const sexpr::SExpr& expr) {
        if (expr.hasChildren() && expr.children().size() == 2) {
            const std::string& target = expr[1].value();
            if (!Utils::hasDollarPrefix(target)) {
                throw MalformedTargetExpr(expr.toString());
            }

            const std::string& value = expr[0].value();
            if (value == "br") {
                return makeNormalBranch(target);
            } else if (value == "case") {
                return makeCaseBranch(target);
            } else {
                throw MalformedTargetExpr(expr.toString());
            }
        } else {
            throw MalformedTargetExpr(expr.toString());
        }
    }

    TableSwitchTarget TableSwitchTarget::makeNormalBranch(const std::string& targetName) {
        TableSwitchTarget result(false, targetName);
        return result;
    }

    TableSwitchTarget TableSwitchTarget::makeCaseBranch(const std::string& targetName) {
        TableSwitchTarget result(true, targetName);
        return result;
    }

    std::size_t TableSwitchTarget::index() const {
        return index_;
    }

    void TableSwitchTarget::index(std::size_t index) {
        index_ = index;
    }

    void TableSwitch::secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {
        std::size_t index = 0;
        for (const Instruction* child : children()) {
            if (index != 0){
                if (child->id() != InstructionId::Case) {
                    throw IllegalNonCaseExpression(child->toSExprString() + " in tableswitch " + toSExprString());
                }
            }
            index++;
        }

        bool hasBranchTarget = false;

        for (TableSwitchTarget& target : targets_) {
            if (target.isCase()) {
                index = 0;
                bool foundFittingChild = false;
                for (const Instruction* child : children()) {
                    if (index != 0) {
                        const Case &caseExpr = dynamic_cast<const Case &>(*child);

                        if (target.targetName() == caseExpr.labelName()) {
                            target.index(index);
                            foundFittingChild = true;
                        }
                    }
                    index++;
                }
                if (!foundFittingChild) {
                    throw CantFindFittingCaseLabel(target.targetName());
                }
            } else if (target.isBranch()) {
                hasBranchTarget = true;
            } else {
                throw std::domain_error("Only branch and case targets are supported");
            }
        }
        if (defaultTarget_.isCase()) {
            index = 0;
            bool foundFittingChild = false;
            for (const Instruction* child : children()) {
                if (index != 0) {
                    const Case &caseExpr = dynamic_cast<const Case &>(*child);

                    if (defaultTarget_.targetName() == caseExpr.labelName()) {
                        defaultTarget_.index(index);
                        foundFittingChild = true;
                    }
                }
                index++;
            }
            if (!foundFittingChild) {
                throw CantFindFittingCaseLabel(defaultTarget_.targetName());
            }
        } else if (defaultTarget_.isBranch()) {
            hasBranchTarget = true;
        } else {
            throw std::domain_error("Only branch and case targets are supported");
        }


        if (children().size() != 1)
            returnType_ = children().back()->returnType();

        const Type* branchType = BranchTypeValidator::checkBranchType(*this);
        if (branchType) {
            if (returnType_ != branchType) {
                returnType_ = Void::instance();
            }
        }

        if (hasBranchTarget) {
            //returnType_ = Void::instance();
        }
    }

    void Label::secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) {
        returnType_ = children().front()->returnType();
        const Type* branchType = BranchTypeValidator::checkBranchType(*this);
        if (branchType) {
            if (returnType_ != branchType) {
                returnType_ = Void::instance();
            }
        }
    }
}
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

#ifndef WASMINT_INSTRUCTIONS_H
#define WASMINT_INSTRUCTIONS_H

#include <types/Void.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float64.h>
#include <types/Float32.h>
#include <FunctionContext.h>
#include <sexpr_parsing/SExpr.h>
#include <ModuleContext.h>
#include <functional>
#include <Utils.h>
#include "Instruction.h"
#include "InstructionId.h"
#include "UnreachableValidator.h"
#include <set>
#include <branching/BranchInformation.h>

namespace wasm_module {

    ExceptionMessage(OverflowInOffsetAttribute)
    ExceptionMessage(OverflowInAlignAttribute)
    ExceptionMessage(AlignNotPowerOfTwo)

    #define DeclInstComma ,

    #define DeclInstruction(CLASS_NAME, STR, CHILDREN, RETURN_TYPE) class CLASS_NAME : public Instruction { \
        virtual const std::string& name() const override { static std::string name_ = STR ; return name_; } \
        virtual InstructionId::Value id() const override { return InstructionId:: CLASS_NAME ; } \
        virtual const Type* returnType() const override { return RETURN_TYPE ; } \
        virtual const std::vector<const Type *>& childrenTypes() const override { static std::vector<const Type *> chTypes_ = CHILDREN ; return chTypes_; }

    #define DeclLoadStoreInstruction(CLASS_NAME, STR, CHILDREN, RETURN_TYPE) class CLASS_NAME : public LoadStoreInstruction { \
        public: virtual const std::string& name() const override { static std::string name_ = STR ; return name_; } \
        virtual InstructionId::Value id() const override { return InstructionId:: CLASS_NAME ; } \
        virtual const Type* returnType() const override { return RETURN_TYPE ; } \
        virtual const std::vector<const Type *>& childrenTypes() const override { static std::vector<const Type *> chTypes_ = CHILDREN ; return chTypes_; } \
        CLASS_NAME (const sexpr::SExpr expr) : LoadStoreInstruction(expr) { }


    DeclInstruction(I32Add, "i32.add", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32Sub, "i32.sub", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32Mul, "i32.mul", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32DivSigned, "i32.div_s", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32DivUnsigned, "i32.div_u", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32RemainderSigned, "i32.rem_s", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32RemainderUnsigned, "i32.rem_u", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32And, "i32.and", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32Or, "i32.or", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32Xor, "i32.xor", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32ShiftLeft, "i32.shl", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32ShiftRightZeroes, "i32.shr_u", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32ShiftRightSigned, "i32.shr_s", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32Equal, "i32.eq", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32NotEqual, "i32.ne", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32LessThanSigned, "i32.lt_s", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32LessEqualSigned, "i32.le_s", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32LessThanUnsigned, "i32.lt_u", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32LessEqualUnsigned, "i32.le_u", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32GreaterThanSigned, "i32.gt_s", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32GreaterEqualSigned, "i32.ge_s", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32GreaterThanUnsigned, "i32.gt_u", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32GreaterEqualUnsigned, "i32.ge_u", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclInstruction(I32CountLeadingZeroes, "i32.clz", {Int32::instance()}, Int32::instance())};
    DeclInstruction(I32CountTrailingZeroes, "i32.ctz", {Int32::instance()}, Int32::instance())};
    DeclInstruction(I32PopulationCount, "i32.popcnt", {Int32::instance()}, Int32::instance())};

    DeclInstruction(I64Add, "i64.add", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64Sub, "i64.sub", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64Mul, "i64.mul", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64DivSigned, "i64.div_s", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64DivUnsigned, "i64.div_u", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64RemainderSigned, "i64.rem_s", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64RemainderUnsigned, "i64.rem_u", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64And, "i64.and", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64Or, "i64.or", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64Xor, "i64.xor", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64ShiftLeft, "i64.shl", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64ShiftRightZeroes, "i64.shr_u", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64ShiftRightSigned, "i64.shr_s", {Int64::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclInstruction(I64Equal, "i64.eq", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64NotEqual, "i64.ne", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64LessThanSigned, "i64.lt_s", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64LessEqualSigned, "i64.le_s", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64LessThanUnsigned, "i64.lt_u", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64LessEqualUnsigned, "i64.le_u", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64GreaterThanSigned, "i64.gt_s", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64GreaterEqualSigned, "i64.ge_s", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64GreaterThanUnsigned, "i64.gt_u", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64GreaterEqualUnsigned, "i64.ge_u", {Int64::instance() DeclInstComma Int64::instance()}, Int32::instance())};
    DeclInstruction(I64CountLeadingZeroes, "i64.clz", {Int64::instance()}, Int64::instance())};
    DeclInstruction(I64CountTrailingZeroes, "i64.ctz", {Int64::instance()}, Int64::instance())};
    DeclInstruction(I64PopulationCount, "i64.popcnt", {Int64::instance()}, Int64::instance())};

    DeclInstruction(AddressOf, "address_of", {}, Void::instance())};

    DeclInstruction(Return, "return", {Void::instance()}, Void::instance())};

    DeclInstruction(GrowMemory, "grow_memory", {Int32::instance()}, Int32::instance())};
    DeclInstruction(PageSize, "page_size", {}, Int32::instance())};
    DeclInstruction(CurrentMemory, "current_memory", {}, Int32::instance())};

    class LoadStoreInstruction : public Instruction {

        uint32_t offset_ = 0;
        uint32_t align_ = 0;

    public:
        LoadStoreInstruction(const wasm_module::sexpr::SExpr& expr);

        uint32_t offset() const {
            return offset_;
        }
    };

    DeclLoadStoreInstruction(I32Load8Signed, "i32.load8_s", {Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I32Load8Unsigned, "i32.load8_u", {Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I32Load16Signed, "i32.load16_s", {Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I32Load16Unsigned, "i32.load16_u", {Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I32Load, "i32.load", {Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I64Load8Signed, "i64.load8_s", {Int32::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Load8Unsigned, "i64.load8_u", {Int32::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Load16Signed, "i64.load16_s", {Int32::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Load16Unsigned, "i64.load16_u", {Int32::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Load32Signed, "i64.load32_s", {Int32::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Load32Unsigned, "i64.load32_u", {Int32::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Load, "i64.load", {Int32::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(F32Load, "f32.load", {Int32::instance()}, Float32::instance())};
    DeclLoadStoreInstruction(F64Load, "f64.load", {Int32::instance()}, Float64::instance())};

    DeclLoadStoreInstruction(I32Store8, "i32.store8", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I32Store16, "i32.store16", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I32Store, "i32.store", {Int32::instance() DeclInstComma Int32::instance()}, Int32::instance())};
    DeclLoadStoreInstruction(I64Store8, "i64.store8", {Int32::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Store16, "i64.store16", {Int32::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Store32, "i64.store32", {Int32::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(I64Store, "i64.store", {Int32::instance() DeclInstComma Int64::instance()}, Int64::instance())};
    DeclLoadStoreInstruction(F32Store, "f32.store", {Int32::instance() DeclInstComma Float32::instance()}, Float32::instance())};
    DeclLoadStoreInstruction(F64Store, "f64.store", {Int32::instance() DeclInstComma Float64::instance()}, Float64::instance())};


    DeclInstruction(I32Wrap, "i32.wrap/i64", {Int64::instance()}, Int32::instance())};
    DeclInstruction(I32TruncSignedF32, "i32.trunc_s/f32", {Float32::instance()}, Int32::instance())};
    DeclInstruction(I32TruncSignedF64, "i32.trunc_s/f64", {Float64::instance()}, Int32::instance())};
    DeclInstruction(I32TruncUnsignedF32, "i32.trunc_u/f32", {Float32::instance()}, Int32::instance())};
    DeclInstruction(I32TruncUnsignedF64, "i32.trunc_u/f64", {Float64::instance()}, Int32::instance())};
    DeclInstruction(I32ReinterpretF32, "i32.reinterpret/f32", {Float32::instance()}, Int32::instance())};

    DeclInstruction(I64ExtendSignedI32, "i64.extend_s/i32", {Int32::instance()}, Int64::instance())};
    DeclInstruction(I64ExtendUnsignedI32, "i64.extend_u/i32", {Int32::instance()}, Int64::instance())};
    DeclInstruction(I64TruncSignedF32, "i64.trunc_s/f32", {Float32::instance()}, Int64::instance())};
    DeclInstruction(I64TruncSignedF64, "i64.trunc_s/f64", {Float64::instance()}, Int64::instance())};
    DeclInstruction(I64TruncUnsignedF32, "i64.trunc_u/f32", {Float32::instance()}, Int64::instance())};
    DeclInstruction(I64TruncUnsignedF64, "i64.trunc_u/f64", {Float64::instance()}, Int64::instance())};
    DeclInstruction(I64ReinterpretF64, "i64.reinterpret/f64", {Float64::instance()}, Int64::instance())};

    DeclInstruction(F32DemoteF64, "f32.demote/f64", {Float64::instance()}, Float32::instance())};
    DeclInstruction(F32ConvertSignedI32, "f32.convert_s/i32", {Int32::instance()}, Float32::instance())};
    DeclInstruction(F32ConvertSignedI64, "f32.convert_s/i64", {Int64::instance()}, Float32::instance())};
    DeclInstruction(F32ConvertUnsignedI32, "f32.convert_u/i32", {Int32::instance()}, Float32::instance())};
    DeclInstruction(F32ConvertUnsignedI64, "f32.convert_u/i64", {Int64::instance()}, Float32::instance())};
    DeclInstruction(F32ReinterpretI32, "f32.reinterpret/i32", {Int32::instance()}, Float32::instance())};

    DeclInstruction(F64PromoteF32, "f64.promote/f32", {Float32::instance()}, Float64::instance())};
    DeclInstruction(F64ConvertSignedI32, "f64.convert_s/i32", {Int32::instance()}, Float64::instance())};
    DeclInstruction(F64ConvertSignedI64, "f64.convert_s/i64", {Int64::instance()}, Float64::instance())};
    DeclInstruction(F64ConvertUnsignedI32, "f64.convert_u/i32", {Int32::instance()}, Float64::instance())};
    DeclInstruction(F64ConvertUnsignedI64, "f64.convert_u/i64", {Int64::instance()}, Float64::instance())};
    DeclInstruction(F64ReinterpretI64, "f64.reinterpret/i64", {Int64::instance()}, Float64::instance())};

    DeclInstruction(F32Add, "f32.add", {Float32::instance() DeclInstComma Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Sub, "f32.sub", {Float32::instance() DeclInstComma Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Mul, "f32.mul", {Float32::instance() DeclInstComma Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Div, "f32.div", {Float32::instance() DeclInstComma Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Abs, "f32.abs", {Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Neg, "f32.neg", {Float32::instance()}, Float32::instance())};
    DeclInstruction(F32CopySign, "f32.copysign", {Float32::instance() DeclInstComma Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Ceil, "f32.ceil", {Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Floor, "f32.floor", {Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Trunc, "f32.trunc", {Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Nearest, "f32.nearest", {Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Equal, "f32.eq", {Float32::instance() DeclInstComma Float32::instance()}, Int32::instance())};
    DeclInstruction(F32NotEqual, "f32.ne", {Float32::instance() DeclInstComma Float32::instance()}, Int32::instance())};
    DeclInstruction(F32LesserThan, "f32.lt", {Float32::instance() DeclInstComma Float32::instance()}, Int32::instance())};
    DeclInstruction(F32LesserEqual, "f32.le", {Float32::instance() DeclInstComma Float32::instance()}, Int32::instance())};
    DeclInstruction(F32GreaterThan, "f32.gt", {Float32::instance() DeclInstComma Float32::instance()}, Int32::instance())};
    DeclInstruction(F32GreaterEqual, "f32.ge", {Float32::instance() DeclInstComma Float32::instance()}, Int32::instance())};
    DeclInstruction(F32Sqrt, "f32.sqrt", {Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Min, "f32.min", {Float32::instance() DeclInstComma Float32::instance()}, Float32::instance())};
    DeclInstruction(F32Max, "f32.max", {Float32::instance() DeclInstComma Float32::instance()}, Float32::instance())};

    DeclInstruction(F64Add, "f64.add", {Float64::instance() DeclInstComma Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Sub, "f64.sub", {Float64::instance() DeclInstComma Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Mul, "f64.mul", {Float64::instance() DeclInstComma Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Div, "f64.div", {Float64::instance() DeclInstComma Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Abs, "f64.abs", {Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Neg, "f64.neg", {Float64::instance()}, Float64::instance())};
    DeclInstruction(F64CopySign, "f64.copysign", {Float64::instance() DeclInstComma Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Ceil, "f64.ceil", {Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Floor, "f64.floor", {Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Trunc, "f64.trunc", {Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Nearest, "f64.nearest", {Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Equal, "f64.eq", {Float64::instance() DeclInstComma Float64::instance()}, Int32::instance())};
    DeclInstruction(F64NotEqual, "f64.ne", {Float64::instance() DeclInstComma Float64::instance()}, Int32::instance())};
    DeclInstruction(F64LesserThan, "f64.lt", {Float64::instance() DeclInstComma Float64::instance()}, Int32::instance())};
    DeclInstruction(F64LesserEqual, "f64.le", {Float64::instance() DeclInstComma Float64::instance()}, Int32::instance())};
    DeclInstruction(F64GreaterThan, "f64.gt", {Float64::instance() DeclInstComma Float64::instance()}, Int32::instance())};
    DeclInstruction(F64GreaterEqual, "f64.ge", {Float64::instance() DeclInstComma Float64::instance()}, Int32::instance())};
    DeclInstruction(F64Sqrt, "f64.sqrt", {Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Min, "f64.min", {Float64::instance() DeclInstComma Float64::instance()}, Float64::instance())};
    DeclInstruction(F64Max, "f64.max", {Float64::instance() DeclInstComma Float64::instance()}, Float64::instance())};

    DeclInstruction(Unreachable, "unreachable", {}, Void::instance())};
    DeclInstruction(Nop, "nop", {}, Void::instance())};

    class If : public Instruction {

    public:
        virtual const std::string& name() const override {
            static std::string name_ = "if";
            return name_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::If;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type *> chTypes_ = {Int32::instance(), Void::instance()} ; return chTypes_;
        }

        virtual const Type* returnType() const override {
            return Void::instance();
        }
    };

    class IfElse : public Instruction {

    public:
        virtual const std::string& name() const override {
            static std::string name_ = "if_else";
            return name_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::IfElse;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type *> chTypes_ = {Int32::instance(), Void::instance(), Void::instance()} ; return chTypes_;
        }

        virtual const Type* returnType() const override {
            if (children().size() != 3)
                throw std::domain_error("'if_else' without child instructions has no return type");
            if (children()[1]->returnType() == children()[1]->returnType()) {
                return children()[1]->returnType();
            }
            return Void::instance();
        }
    };

    class Select : public Instruction {

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override {
            if (UnreachableValidator::canEvaluate(children()[0]) &&
                children()[0]->returnType() == Void::instance()) {
                throw IncompatibleChildReturnType("First argument of 'select' must not return void: "
                                                  + toSExprString());
            }
            if (UnreachableValidator::canEvaluate(children()[1]) &&
                    children()[1]->returnType() == Void::instance()) {
                throw IncompatibleChildReturnType("Second argument of 'select' must not return void."
                                                  + toSExprString());
            }
            if (UnreachableValidator::canEvaluate(children()[0])
                && UnreachableValidator::canEvaluate(children()[1])
                && children()[0]->returnType() != children()[1]->returnType()) {
                throw IncompatibleChildReturnType("'select' expects two arguments of the same type."
                                                  + toSExprString());
            }
        }

    public:
        virtual const std::string& name() const override {
            static std::string name_ = "select";
            return name_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::Select;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type *> chTypes_ = {Void::instance(), Void::instance(), Int32::instance()} ; return chTypes_;
        }

        virtual const Type* returnType() const override {
            if (children()[1]->returnType() == Void::instance())
                return children()[2]->returnType();
            return children()[1]->returnType();
        }
    };


    class SetLocal : public Instruction {

        std::vector<const Type*> expectedType;
        std::string localName_;

    public:
        uint32_t localIndex;

        SetLocal(binary::ByteStream &stream, FunctionContext &functionContext) {
            localIndex = stream.popULEB128();
            expectedType = {functionContext.locals().at(localIndex)};
        }

        SetLocal(const sexpr::SExpr& expr, FunctionContext &context) {
            localIndex = context.variableNameToIndex(localName_ = expr[1].value());
            expectedType = {context.locals().at(localIndex)};
        }

        virtual const std::string& name() const override {
            static std::string name_ = "set_local";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return expectedType;
        }

        virtual const Type* returnType() const override {
            return expectedType.front();
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::SetLocal;
        }

        virtual std::string dataString() const override {
            std::string result = name();
            if (!localName_.empty())
                result += " " + localName_;
            return result;
        }
    };

    class GetLocal : public Instruction {

        const Type* returnType_;
        std::string localName_;

    public:
        uint32_t localIndex;

        GetLocal(binary::ByteStream &stream, FunctionContext &context) {
            localIndex = stream.popULEB128();
            returnType_ = context.locals().at(localIndex);
        }

        GetLocal(const sexpr::SExpr& expr, FunctionContext &context) {
            localName_ = expr[1].value();

            if (Utils::hasDollarPrefix(localName_)) {
                localIndex = context.variableNameToIndex(localName_);
            } else {
                localIndex = (uint32_t) Utils::strToSizeT(localName_);
            }
            returnType_ = context.locals().at(localIndex);
        }

        virtual const std::string& name() const override {
            static std::string name_ = "get_local";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type*> chTypes_;
            return chTypes_;
        }

        virtual std::string dataString() const override {
            std::string result = name();
            if (!localName_.empty())
                result += " " + localName_;
            return result;
        }

        virtual const Type* returnType() const override {
            return returnType_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::GetLocal;
        }
    };


    class Block : public Instruction {

        const Type* returnType_ = Void::instance();

        std::vector<const Type *> childrenTypes_;

        std::string labelName_;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override;

    public:
        Block(binary::ByteStream &stream) {
            uint32_t amountOfChildren = stream.popULEB128();
            for (uint32_t i = 0; i < amountOfChildren; i++) {
                childrenTypes_.push_back(Void::instance());
            }
        }

        Block(const sexpr::SExpr& expr, FunctionContext &context) {
            if (expr.children().size() >= 2 && expr[1].hasValue()) {
                labelName_ = expr[1].value();
            }
            for (const sexpr::SExpr& subexpr : expr.children()) {
                if (subexpr.hasChildren()) {
                    childrenTypes_.push_back(Void::instance());
                }
            }
        }

        Block(uint32_t amountOfChildren) {
            for (uint32_t i = 0; i < amountOfChildren; i++) {
                childrenTypes_.push_back(Void::instance());
            }
        }

        virtual const std::string& name() const override {
            static std::string name_ = "block";
            return name_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::Block;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return childrenTypes_;
        }

        virtual const Type* returnType() const override {
            return returnType_;
        }

        virtual bool typeCheckChildren() const override {
            return true;
        }

        virtual uint32_t labelCount() const override {
            return 1;
        }

        virtual bool hasLabelName(const std::string& str) const override {
            return str == labelName_;
        }
    };

    class Loop : public Instruction {

        const Type* returnType_ = Void::instance();

        std::vector<const Type*> chTypes_;

        std::string startLabelName_, endLabelName_;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override;

    public:

        Loop(const sexpr::SExpr& expr, FunctionContext &context) {
            if (expr.children().size() >= 2 && expr[1].hasValue()) {
                startLabelName_ = expr[1].value();
            }
            if (expr.children().size() >= 3 && expr[2].hasValue()) {
                endLabelName_ = expr[2].value();
            }
            if (endLabelName_.empty() && !startLabelName_.empty()) {
                endLabelName_ = startLabelName_;
                startLabelName_.clear();
            }

            for (const sexpr::SExpr& subexpr : expr.children()) {
                if (subexpr.hasChildren()) {
                    chTypes_.push_back(Void::instance());
                }
            }
        }

        virtual const std::string& name() const override {
            static std::string name_ = "loop";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return chTypes_;
        }

        virtual const Type* returnType() const override {
            return returnType_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::Loop;
        }

        virtual uint32_t labelCount() const override {
            return 2; // TODO this is just for ml-proto compat, increase to two somewhen
        }

        virtual bool hasLabelName(const std::string& str) const override {
            return str == startLabelName_ || str == endLabelName_;
        }

        virtual uint32_t labelIndex(const std::string& str) const override {
            if (str == startLabelName_)
                return 1;
            if (str == endLabelName_)
                return 0;
            throw std::domain_error("labelIndex(\"" + str + "\") can't be executed on this loop instruction.");
        }
    };


    class Label : public Instruction {

        const Type* returnType_ = Void::instance();
        std::string labelName_;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override;

    public:

        Label(const sexpr::SExpr& expr, FunctionContext &context) {
            if (expr.children().size() >= 2 && expr[1].hasValue()) {
                labelName_ = expr[1].value();
            }
        }

        virtual const std::string& name() const override {
            static std::string name_ = "label";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type*> chTypes_ = {Void::instance()};
            return chTypes_;
        }

        virtual const Type* returnType() const override {
            return returnType_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::Label;
        }

        virtual uint32_t labelCount() const override {
            return 1;
        }

        virtual bool hasLabelName(const std::string& str) const override {
            return str == labelName_;
        }
    };


    class Branch : public Instruction {

        std::string labelName_;
        uint32_t branchLabel_;
        BranchInformation branchInformation_;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override;

    public:

        Branch(const sexpr::SExpr& expr, FunctionContext &context) {
            labelName_ = expr[1].value();
            if (!Utils::hasDollarPrefix(labelName_)) {
                branchLabel_ = (uint32_t) Utils::strToSizeT(labelName_);
                labelName_.clear();
            }
        }

        virtual const std::string& name() const override {
            static std::string name_ = "br";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type*> chTypes_ = {Void::instance()};
            return chTypes_;
        }

        virtual const Type* returnType() const override {
            return Void::instance();
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::Branch;
        }

        using Instruction::children;

        virtual void children(const std::vector<Instruction*>& newChildren) override {
            if (newChildren.empty()) {
                Instruction::children({new Nop()});
            } else {
                Instruction::children(newChildren);
            }
        }

        virtual std::string dataString() const override {
            std::string result = name();
            result += " ";
            if (labelName_.empty())
                result += std::to_string(branchInformation_.label());
            else
                result += labelName_;
            return result;
        }

        uint32_t branchLabel() const {
            return branchInformation_.label();
        }

        const Instruction& getBranchTarget() const {
            return *branchInformation_.target();
        }

        virtual const BranchInformation* branchInformation() const override {
            return &branchInformation_;
        }
    };


    class BranchIf : public Instruction {

        uint32_t branchLabel_ = 0;
        std::string labelName_;
        BranchInformation branchInformation_;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override;

    public:

        BranchIf(const sexpr::SExpr& expr, FunctionContext &context) {
            labelName_ = expr[1].value();
            if (!Utils::hasDollarPrefix(labelName_)) {
                branchLabel_ = (uint32_t) Utils::strToSizeT(labelName_);
                labelName_.clear();
            }
        }

        virtual const std::string& name() const override {
            static std::string name_ = "br_if";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type*> chTypes_ = {Int32::instance(), Void::instance()};
            return chTypes_;
        }

        virtual const Type* returnType() const override {
            return Void::instance();
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::BranchIf;
        }

        virtual std::string dataString() const override {
            std::string result = name();
            result += " ";
            if (labelName_.empty())
                result += std::to_string(branchInformation_.label());
            else
                result += labelName_;
            return result;
        }

        virtual void children(const std::vector<Instruction*>& newChildren) override {
            if (newChildren.size() == 1) {
                Instruction::children({newChildren.at(0), new Nop()});
            } else {
                Instruction::children(newChildren);
            }
        }

        using Instruction::children;

        uint32_t branchLabel() const {
            return branchInformation_.label();
        }

        virtual const BranchInformation* branchInformation() const override {
            return &branchInformation_;
        }
    };

    class NativeInstruction : public Instruction {

        std::function<Variable(std::vector<Variable>)> internalFunction_;
        std::vector<const Type*> parameterTypes_;
        const Type* returnType_;

    public:
        NativeInstruction(std::function<Variable(std::vector<Variable>)> internalFunction, const Type* returnType, std::vector<const Type*> parameterTypes)
                : internalFunction_(internalFunction), parameterTypes_(parameterTypes), returnType_(returnType)
        {
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return parameterTypes_;
        }

        virtual const std::string& name() const override {
            static std::string name_ = "native";
            return name_;
        }

        virtual const Type* returnType() const override {
            return returnType_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::NativeInstruction;
        }

        Variable call(std::vector<Variable> parameters) const {
            return internalFunction_(parameters);
        }
    };

    class Literal : public Instruction {

        std::string literalValueStr_;
        Variable literalValue_;
    public:
        Variable literalValue() const {
            return literalValue_;
        }

        Literal(binary::ByteStream &stream, ModuleContext &context) {
            uint32_t typeId = stream.popULEB128();

            const Type* type = context.typeTable().getType(typeId);

            literalValue_ = Variable(type);
            type->parse(stream, literalValue_.value());
        }

        Literal(const sexpr::SExpr& expr);

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type*> chTypes_;
            return chTypes_;
        }

        virtual const std::string& name() const override {
            if (&literalValue_.type() == Int32::instance()) {
                static std::string namei32_ = "i32.const";
                return namei32_;
            }
            if (&literalValue_.type() == Int64::instance()) {
                static std::string namei64_ = "i64.const";
                return namei64_;
            }
            if (&literalValue_.type() == Float32::instance()) {
                static std::string namef32_ = "f32.const";
                return namef32_;
            }
            if (&literalValue_.type() == Float64::instance()) {
                static std::string namef64_ = "f64.const";
                return namef64_;
            }
            if (&literalValue_.type() == Void::instance()) {
                throw std::domain_error("Literal type is void. That should not happen.");
            }
            throw std::domain_error("Unknown literal type");
        }


        virtual std::string dataString() const override {
            std::string result = name();
            result += " ";
            result += literalValueStr_;
            return result;
        }

        virtual const Type* returnType() const override {
            return &literalValue_.type();
        }

        virtual InstructionId::Value id() const override {
            if (&literalValue_.type() == Int32::instance()) {
                return InstructionId::I32Const;
            }
            if (&literalValue_.type() == Int64::instance()) {
                return InstructionId::I64Const;
            }
            if (&literalValue_.type() == Float32::instance()) {
                return InstructionId::F32Const;
            }
            if (&literalValue_.type() == Float64::instance()) {
                return InstructionId::F64Const;
            }
            if (&literalValue_.type() == Void::instance()) {
                throw std::domain_error("Literal type is void. That should not happen.");
            }
            throw std::domain_error("Unknown literal type");
        }
    };

    ExceptionMessage(CaseStatementIsNotDirectChildOfTableswitch)

    class Case : public Instruction {

        std::vector<const Type*> caseChildren_;
        const Type* returnType_ = Void::instance();
        std::string labelName_;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override {
            returnType_ = children().back()->returnType();
            if (hasParent()) {
                if (parent()->id() != InstructionId::TableSwitch) {
                    throw CaseStatementIsNotDirectChildOfTableswitch("case statement parent is " + parent()->dataString()
                                                                     + " but needs to be direct child of tableswitch");
                }
            } else {
                throw CaseStatementIsNotDirectChildOfTableswitch("case statement has no parent but needs to be direct "
                                                                         "child of tableswitch");
            }
        }

    public:

        Case(const sexpr::SExpr& expr, FunctionContext &context) {
            if (expr.children().size() >= 2 && expr[1].hasValue()) {
                labelName_ = expr[1].value();
            }
            for (const sexpr::SExpr& subExpr : expr.children()) {
                if (subExpr.hasChildren())
                    caseChildren_.push_back(Void::instance());
            }
            if (caseChildren_.empty()) {
                caseChildren_.push_back(Void::instance());
            }
        }

        virtual const std::string& name() const override {
            static std::string name_ = "case";
            return name_;
        }

        using Instruction::children;

        virtual void children(const std::vector<Instruction*>& newChildren) override {
            if (newChildren.empty()) {
                Instruction::children({new Nop()});
            } else {
                Instruction::children(newChildren);
            }
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return caseChildren_;
        }

        virtual std::string dataString() const override {
            std::string result = name();
            if (!labelName_.empty())
                result += " " + labelName_;
            return result;
        }

        virtual const Type* returnType() const override {
            return returnType_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::Case;
        }

        const std::string& labelName() const {
            return labelName_;
        }
    };

    ExceptionMessage(MultipleTableExprsInTableSwitch)
    ExceptionMessage(NoTableExprInTableSwitch)
    ExceptionMessage(MalformedTargetExpr)
    ExceptionMessage(IllegalNonCaseExpression)

    class TableSwitchTarget {
        bool isCase_ = 0;
        std::string targetName_;
        std::size_t index_ = 0;
        BranchInformation branchInformation_;

    public:
        TableSwitchTarget() {
        }
        TableSwitchTarget(bool isCase, const std::string& targetName) : isCase_(isCase), targetName_(targetName) {
        }

        static TableSwitchTarget parse(const sexpr::SExpr& expr);
        static TableSwitchTarget makeNormalBranch(const std::string& targetName);
        static TableSwitchTarget makeCaseBranch(const std::string& targetName);

        bool isCase() const;
        bool isBranch() const;
        const std::string & targetName() const;
        std::size_t index() const;
        void index(std::size_t index);
        void branchInformation(const BranchInformation& information) {
            branchInformation_ = information;
        }

        const BranchInformation& branchInformation() const {
            return branchInformation_;
        }
    };

    ExceptionMessage(CantFindFittingCaseLabel)

    class TableSwitch : public Instruction {

        const Type* returnType_ = Void::instance();
        TableSwitchTarget defaultTarget_;
        std::vector<TableSwitchTarget> targets_;
        std::vector<const Type*> childrenTypes_;
        std::string labelName_;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override;

    public:

        TableSwitch(const sexpr::SExpr& expr, std::set<std::size_t>& subExprsToIgnore);

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return childrenTypes_;
        }

        virtual const std::string& name() const override {
            static std::string name_ = "tableswitch";
            return name_;
        }

        virtual std::string dataString() const override {
            std::string result = name();
            return result;
        }

        virtual const Type* returnType() const override {
            return returnType_;
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::TableSwitch;
        }

        const std::vector<TableSwitchTarget>& targets() const {
            return targets_;
        }

        virtual uint32_t labelCount() const override {
            return 1;
        }

        virtual bool hasLabelName(const std::string& str) const override {
            return str == labelName_;
        }

        const TableSwitchTarget& defaultTarget() const {
            return defaultTarget_;
        }
    };


    class HasFeature : public Instruction {

        std::string featureName_;

    public:
        const std::string& featureName() const {
            return featureName_;
        }

        HasFeature(const sexpr::SExpr& expr) {
            featureName_ = expr[1].value();
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            static std::vector<const Type*> chTypes_;
            return chTypes_;
        }

        virtual const std::string& name() const override {
            static std::string name_ = "has_feature";
            return name_;
        }


        virtual std::string dataString() const override {
            return name();
        }

        virtual const Type* returnType() const override {
            return Int32::instance();
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::HasFeature;
        }
    };

    class Module;

    class Call : public Instruction {

        std::string functionName;

    protected:

        virtual void secondStepEvaluate(ModuleContext& context, FunctionContext& functionContext) override {
            functionSignature = context.mainFunctionTable().getFunctionSignature(functionName);
            moduleName = context.name();
        }

    public:
        std::string moduleName;
        FunctionSignature functionSignature;

        Call(binary::ByteStream &stream, ModuleContext &context) {
            functionSignature = context.mainFunctionTable().getFunctionSignature(stream.popULEB128());
            moduleName = context.name();
        }

        Call(const sexpr::SExpr& expr, ModuleContext &context) {
            functionName = expr[1].value();
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::Call;
        }

        virtual const std::string& name() const override {
            static std::string name_ = "call";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return functionSignature.parameters();
        }

        virtual const Type* returnType() const override {
            return functionSignature.returnType();
        }

        virtual std::string dataString() const override {
            return name() + " " + functionName;
        }
    };

    class CallIndirect : public Instruction {

        std::vector<const Type*> childrenTypes_ = {Int32::instance()};
        FunctionType functionType_;

    public:
        ModuleContext* context_;

        CallIndirect(const sexpr::SExpr& expr, ModuleContext &context) : context_(&context) {
            std::string typeValue = expr[1].value();

            if (Utils::hasDollarPrefix(typeValue)) {
                functionType_ = context.functionTypeTable().getType(typeValue);
            } else {
                functionType_ = context.functionTypeTable().getType(Utils::strToSizeT(typeValue));
            }
            for (const Type* parameter : functionType_.parameters()) {
                childrenTypes_.push_back(parameter);
            }
        }

        virtual InstructionId::Value id() const override {
            return InstructionId::CallIndirect;
        }

        virtual const std::string& name() const override {
            static std::string name_ = "call_indirect";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return childrenTypes_;
        }

        virtual const Type* returnType() const override {
            return functionType_.returnType();
        }

        virtual bool typeCheckChildren() const override {
            return false;
        }

        virtual std::string dataString() const override {
            return name();
        }

        const FunctionType& functionType() const {
            return functionType_;
        }
    };

    class CallImport : public Instruction {

        std::size_t functionIndex;
        std::string functionName;

    protected:

        virtual void secondStepEvaluate(ModuleContext &context, FunctionContext& functionContext) override {
            if (context.name().empty()) {
                functionSignature = context.importedFunctionTable().getFunctionSignature(functionIndex);
            } else {
                functionSignature = context.importedFunctionTable().getFunctionSignature(functionName);
            }
        }

    public:
        FunctionSignature functionSignature;

        CallImport(binary::ByteStream &stream, ModuleContext &context) {
            functionSignature = context.importedFunctionTable().getFunctionSignature(stream.popULEB128());
        }

        CallImport(const sexpr::SExpr& expr, ModuleContext &context) {
            std::string argument = expr[1].value();

            if (Utils::hasDollarPrefix(argument)) {
                functionName = expr[1].value();
            } else {
                functionIndex = Utils::strToSizeT(argument);
            }

        }

        virtual InstructionId::Value id() const override {
            return InstructionId::CallImport;
        }

        virtual const std::string& name() const override {
            static std::string name_ = "call_import";
            return name_;
        }

        virtual const std::vector<const Type*>& childrenTypes() const override {
            return functionSignature.parameters();
        }

        virtual const Type* returnType() const override {
            return functionSignature.returnType();
        }

        virtual bool typeCheckChildren() const override {
            return false;
        }
    };

}

#endif //WASMINT_INSTRUCTIONS_H

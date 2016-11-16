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



#include "InstructionSet.h"
#include "Instructions.h"

namespace wasm_module {

    inline bool ends_with(std::string const & value, std::string const & ending)
    {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    Instruction *InstructionSet::getInstruction(std::string name, binary::ByteStream &stream, ModuleContext &context,
                                                FunctionContext &functionContext) {
        Instruction* instruction = getInstruction(name, context, functionContext);

        if (instruction) {
            return instruction;
        } else {
            if (name == "literal") {
                return new Literal(stream, context);
            } else if (name == "call") {
                return new Call(stream, context);
            } else if (name == "get_local") {
                return new GetLocal(stream, functionContext);
            } else if (name == "block") {
                return new Block(stream);
            } else if (name == "set_local") {
                return new SetLocal(stream, functionContext);
            } else {
                throw UnknownInstructionName(name);
            }
        }

    }

#define SExprLoadStoreInstruction(CLASSNAME, INSTRNAME) if(name==INSTRNAME){return new CLASSNAME(expr);}

    Instruction *InstructionSet::getInstruction(std::string name, const sexpr::SExpr& expr, ModuleContext &context,
                                                       FunctionContext &functionContext, std::set<std::size_t>& subExprsToIgnore) {

        Instruction* instruction = getInstruction(name, context, functionContext);

        if (instruction) {
            return instruction;
        } else {
            if (name == "call") {
                return new Call(expr, context);
            } else if (name == "call_import") {
                return new CallImport(expr, context);
            } else if (name == "call_indirect") {
                return new CallIndirect(expr, context);
            } else if (name == "get_local") {
                return new GetLocal(expr, functionContext);
            } else if (name == "set_local") {
                return new SetLocal(expr, functionContext);
            } else if (name == "tee_local") {
                return new TeeLocal(expr, functionContext);
            } else if (name == "has_feature") {
                return new HasFeature(expr);
            } else if (name == "label") {
                return new Label(expr, functionContext);
            } else if (name == "loop") {
                return new Loop(expr, functionContext);
            } else if (name == "block") {
                return new Block(expr, functionContext);
            } else if (name == "drop") {
                return new Drop(expr);
            } else if (name == "tableswitch") {
                return new TableSwitch(expr, subExprsToIgnore);
            } else if (name == "case") {
                return new Case(expr, functionContext);
            } else if (name == "br") {
                return new Branch(expr, functionContext);
            } else if (name == "br_if") {
                return new BranchIf(expr, functionContext);
            } else if (ends_with(name, ".const")) {
                return new Literal(expr);
            }
            else SExprLoadStoreInstruction(I32Load8Signed, "i32.load8_s")
            else SExprLoadStoreInstruction(I32Load8Unsigned, "i32.load8_u")
            else SExprLoadStoreInstruction(I32Load16Signed, "i32.load16_s")
            else SExprLoadStoreInstruction(I32Load16Unsigned, "i32.load16_u")
            else SExprLoadStoreInstruction(I32Load, "i32.load")
            else SExprLoadStoreInstruction(I64Load8Signed, "i64.load8_s")
            else SExprLoadStoreInstruction(I64Load8Unsigned, "i64.load8_u")
            else SExprLoadStoreInstruction(I64Load16Signed, "i64.load16_s")
            else SExprLoadStoreInstruction(I64Load16Unsigned, "i64.load16_u")
            else SExprLoadStoreInstruction(I64Load32Signed, "i64.load32_s")
            else SExprLoadStoreInstruction(I64Load32Unsigned, "i64.load32_u")
            else SExprLoadStoreInstruction(I64Load, "i64.load")
            else SExprLoadStoreInstruction(F32Load, "f32.load")
            else SExprLoadStoreInstruction(F64Load, "f64.load")

            else SExprLoadStoreInstruction(I32Store8, "i32.store8")
            else SExprLoadStoreInstruction(I32Store16, "i32.store16")
            else SExprLoadStoreInstruction(I32Store, "i32.store")
            else SExprLoadStoreInstruction(I64Store8, "i64.store8")
            else SExprLoadStoreInstruction(I64Store16, "i64.store16")
            else SExprLoadStoreInstruction(I64Store32, "i64.store32")
            else SExprLoadStoreInstruction(I64Store, "i64.store")
            else SExprLoadStoreInstruction(F32Store, "f32.store")
            else SExprLoadStoreInstruction(F64Store, "f64.store")
            else {
                throw UnknownInstructionName(name);
            }
        }
    }

    #define INSTRUCTION(CLASSNAME, INSTRNAME) if(name==INSTRNAME){return new CLASSNAME();}

    Instruction* InstructionSet::getInstruction(std::string name, ModuleContext &context,
                                                FunctionContext &functionContext) {
        INSTRUCTION(I32Add, "i32.add")
        INSTRUCTION(I32Sub, "i32.sub")
        INSTRUCTION(I32Mul, "i32.mul")
        INSTRUCTION(I32DivSigned, "i32.div_s")
        INSTRUCTION(I32DivUnsigned, "i32.div_u")
        INSTRUCTION(I32RemainderSigned, "i32.rem_s")
        INSTRUCTION(I32RemainderUnsigned, "i32.rem_u")
        INSTRUCTION(I32And, "i32.and")
        INSTRUCTION(I32Or, "i32.or")
        INSTRUCTION(I32Xor, "i32.xor")
        INSTRUCTION(I32ShiftLeft, "i32.shl")
        INSTRUCTION(I32ShiftRightZeroes, "i32.shr_u")
        INSTRUCTION(I32ShiftRightSigned, "i32.shr_s")
        INSTRUCTION(I32EqualZero, "i32.eqz")
        INSTRUCTION(I32Equal, "i32.eq")
        INSTRUCTION(I32NotEqual, "i32.ne")
        INSTRUCTION(I32LessThanSigned, "i32.lt_s")
        INSTRUCTION(I32LessEqualSigned, "i32.le_s")
        INSTRUCTION(I32LessThanUnsigned, "i32.lt_u")
        INSTRUCTION(I32LessEqualUnsigned, "i32.le_u")
        INSTRUCTION(I32GreaterThanSigned, "i32.gt_s")
        INSTRUCTION(I32GreaterEqualSigned, "i32.ge_s")
        INSTRUCTION(I32GreaterThanUnsigned, "i32.gt_u")
        INSTRUCTION(I32GreaterEqualUnsigned, "i32.ge_u")
        INSTRUCTION(I32CountLeadingZeroes, "i32.clz")
        INSTRUCTION(I32CountTrailingZeroes, "i32.ctz")
        INSTRUCTION(I32PopulationCount, "i32.popcnt")

        INSTRUCTION(I64Add, "i64.add")
        INSTRUCTION(I64Sub, "i64.sub")
        INSTRUCTION(I64Mul, "i64.mul")
        INSTRUCTION(I64DivSigned, "i64.div_s")
        INSTRUCTION(I64DivUnsigned, "i64.div_u")
        INSTRUCTION(I64RemainderSigned, "i64.rem_s")
        INSTRUCTION(I64RemainderUnsigned, "i64.rem_u")
        INSTRUCTION(I64And, "i64.and")
        INSTRUCTION(I64Or, "i64.or")
        INSTRUCTION(I64Xor, "i64.xor")
        INSTRUCTION(I64ShiftLeft, "i64.shl")
        INSTRUCTION(I64ShiftRightZeroes, "i64.shr_u")
        INSTRUCTION(I64ShiftRightSigned, "i64.shr_s")
        INSTRUCTION(I64EqualZero, "i64.eqz")
        INSTRUCTION(I64Equal, "i64.eq")
        INSTRUCTION(I64NotEqual, "i64.ne")
        INSTRUCTION(I64LessThanSigned, "i64.lt_s")
        INSTRUCTION(I64LessEqualSigned, "i64.le_s")
        INSTRUCTION(I64LessThanUnsigned, "i64.lt_u")
        INSTRUCTION(I64LessEqualUnsigned, "i64.le_u")
        INSTRUCTION(I64GreaterThanSigned, "i64.gt_s")
        INSTRUCTION(I64GreaterEqualSigned, "i64.ge_s")
        INSTRUCTION(I64GreaterThanUnsigned, "i64.gt_u")
        INSTRUCTION(I64GreaterEqualUnsigned, "i64.ge_u")
        INSTRUCTION(I64CountLeadingZeroes, "i64.clz")
        INSTRUCTION(I64CountTrailingZeroes, "i64.ctz")
        INSTRUCTION(I64PopulationCount, "i64.popcnt")

        INSTRUCTION(AddressOf, "address_of")

        INSTRUCTION(If, "if")
        INSTRUCTION(IfElse, "if_else")
        INSTRUCTION(Return, "return")

        INSTRUCTION(GrowMemory, "grow_memory")
        INSTRUCTION(PageSize, "page_size")
        INSTRUCTION(CurrentMemory, "current_memory")

        INSTRUCTION(I32Wrap, "i32.wrap/i64")
        INSTRUCTION(I32TruncSignedF32, "i32.trunc_s/f32")
        INSTRUCTION(I32TruncSignedF64, "i32.trunc_s/f64")
        INSTRUCTION(I32TruncUnsignedF32, "i32.trunc_u/f32")
        INSTRUCTION(I32TruncUnsignedF64, "i32.trunc_u/f64")
        INSTRUCTION(I32ReinterpretF32, "i32.reinterpret/f32")

        INSTRUCTION(I64ExtendSignedI32, "i64.extend_s/i32")
        INSTRUCTION(I64ExtendUnsignedI32, "i64.extend_u/i32")
        INSTRUCTION(I64TruncSignedF32, "i64.trunc_s/f32")
        INSTRUCTION(I64TruncSignedF64, "i64.trunc_s/f64")
        INSTRUCTION(I64TruncUnsignedF32, "i64.trunc_u/f32")
        INSTRUCTION(I64TruncUnsignedF64, "i64.trunc_u/f64")
        INSTRUCTION(I64ReinterpretF64, "i64.reinterpret/f64")

        INSTRUCTION(F32DemoteF64, "f32.demote/f64")
        INSTRUCTION(F32ConvertSignedI32, "f32.convert_s/i32")
        INSTRUCTION(F32ConvertSignedI64, "f32.convert_s/i64")
        INSTRUCTION(F32ConvertUnsignedI32, "f32.convert_u/i32")
        INSTRUCTION(F32ConvertUnsignedI64, "f32.convert_u/i64")
        INSTRUCTION(F32ReinterpretI32, "f32.reinterpret/i32")

        INSTRUCTION(F64PromoteF32, "f64.promote/f32")
        INSTRUCTION(F64ConvertSignedI32, "f64.convert_s/i32")
        INSTRUCTION(F64ConvertSignedI64, "f64.convert_s/i64")
        INSTRUCTION(F64ConvertUnsignedI32, "f64.convert_u/i32")
        INSTRUCTION(F64ConvertUnsignedI64, "f64.convert_u/i64")
        INSTRUCTION(F64ReinterpretI64, "f64.reinterpret/i64")

        INSTRUCTION(Select, "select")

        INSTRUCTION(F32Add, "f32.add")
        INSTRUCTION(F32Sub, "f32.sub")
        INSTRUCTION(F32Mul, "f32.mul")
        INSTRUCTION(F32Div, "f32.div")
        INSTRUCTION(F32Abs, "f32.abs")
        INSTRUCTION(F32Neg, "f32.neg")
        INSTRUCTION(F32CopySign, "f32.copysign")
        INSTRUCTION(F32Ceil, "f32.ceil")
        INSTRUCTION(F32Floor, "f32.floor")
        INSTRUCTION(F32Trunc, "f32.trunc")
        INSTRUCTION(F32Nearest, "f32.nearest")
        INSTRUCTION(F32Equal, "f32.eq")
        INSTRUCTION(F32NotEqual, "f32.ne")
        INSTRUCTION(F32LesserThan, "f32.lt")
        INSTRUCTION(F32LesserEqual, "f32.le")
        INSTRUCTION(F32GreaterThan, "f32.gt")
        INSTRUCTION(F32GreaterEqual, "f32.ge")
        INSTRUCTION(F32Sqrt, "f32.sqrt")
        INSTRUCTION(F32Min, "f32.min")
        INSTRUCTION(F32Max, "f32.max")

        INSTRUCTION(F64Add, "f64.add")
        INSTRUCTION(F64Sub, "f64.sub")
        INSTRUCTION(F64Mul, "f64.mul")
        INSTRUCTION(F64Div, "f64.div")
        INSTRUCTION(F64Abs, "f64.abs")
        INSTRUCTION(F64Neg, "f64.neg")
        INSTRUCTION(F64CopySign, "f64.copysign")
        INSTRUCTION(F64Ceil, "f64.ceil")
        INSTRUCTION(F64Floor, "f64.floor")
        INSTRUCTION(F64Trunc, "f64.trunc")
        INSTRUCTION(F64Nearest, "f64.nearest")
        INSTRUCTION(F64Equal, "f64.eq")
        INSTRUCTION(F64NotEqual, "f64.ne")
        INSTRUCTION(F64LesserThan, "f64.lt")
        INSTRUCTION(F64LesserEqual, "f64.le")
        INSTRUCTION(F64GreaterThan, "f64.gt")
        INSTRUCTION(F64GreaterEqual, "f64.ge")
        INSTRUCTION(F64Sqrt, "f64.sqrt")
        INSTRUCTION(F64Min, "f64.min")
        INSTRUCTION(F64Max, "f64.max")
        INSTRUCTION(Unreachable, "unreachable")
        INSTRUCTION(Nop, "nop")

        return nullptr;
    }
}
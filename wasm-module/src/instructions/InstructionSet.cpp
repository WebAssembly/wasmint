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
            } else if (name == "has_feature") {
                return new HasFeature(expr);
            } else if (name == "label") {
                return new Label(expr, functionContext);
            } else if (name == "loop") {
                return new Loop(expr, functionContext);
            } else if (name == "block") {
                return new Block(expr, functionContext);
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
        else INSTRUCTION(I32Sub, "i32.sub")
        else INSTRUCTION(I32Mul, "i32.mul")
        else INSTRUCTION(I32DivSigned, "i32.div_s")
        else INSTRUCTION(I32DivUnsigned, "i32.div_u")
        else INSTRUCTION(I32RemainderSigned, "i32.rem_s")
        else INSTRUCTION(I32RemainderUnsigned, "i32.rem_u")
        else INSTRUCTION(I32And, "i32.and")
        else INSTRUCTION(I32Or, "i32.or")
        else INSTRUCTION(I32Xor, "i32.xor")
        else INSTRUCTION(I32ShiftLeft, "i32.shl")
        else INSTRUCTION(I32ShiftRightZeroes, "i32.shr_u")
        else INSTRUCTION(I32ShiftRightSigned, "i32.shr_s")
        else INSTRUCTION(I32Equal, "i32.eq")
        else INSTRUCTION(I32NotEqual, "i32.ne")
        else INSTRUCTION(I32LessThanSigned, "i32.lt_s")
        else INSTRUCTION(I32LessEqualSigned, "i32.le_s")
        else INSTRUCTION(I32LessThanUnsigned, "i32.lt_u")
        else INSTRUCTION(I32LessEqualUnsigned, "i32.le_u")
        else INSTRUCTION(I32GreaterThanSigned, "i32.gt_s")
        else INSTRUCTION(I32GreaterEqualSigned, "i32.ge_s")
        else INSTRUCTION(I32GreaterThanUnsigned, "i32.gt_u")
        else INSTRUCTION(I32GreaterEqualUnsigned, "i32.ge_u")
        else INSTRUCTION(I32CountLeadingZeroes, "i32.clz")
        else INSTRUCTION(I32CountTrailingZeroes, "i32.ctz")
        else INSTRUCTION(I32PopulationCount, "i32.popcnt")

        else INSTRUCTION(I64Add, "i64.add")
        else INSTRUCTION(I64Sub, "i64.sub")
        else INSTRUCTION(I64Mul, "i64.mul")
        else INSTRUCTION(I64DivSigned, "i64.div_s")
        else INSTRUCTION(I64DivUnsigned, "i64.div_u")
        else INSTRUCTION(I64RemainderSigned, "i64.rem_s")
        else INSTRUCTION(I64RemainderUnsigned, "i64.rem_u")
        else INSTRUCTION(I64And, "i64.and")
        else INSTRUCTION(I64Or, "i64.or")
        else INSTRUCTION(I64Xor, "i64.xor")
        else INSTRUCTION(I64ShiftLeft, "i64.shl")
        else INSTRUCTION(I64ShiftRightZeroes, "i64.shr_u")
        else INSTRUCTION(I64ShiftRightSigned, "i64.shr_s")
        else INSTRUCTION(I64Equal, "i64.eq")
        else INSTRUCTION(I64NotEqual, "i64.ne")
        else INSTRUCTION(I64LessThanSigned, "i64.lt_s")
        else INSTRUCTION(I64LessEqualSigned, "i64.le_s")
        else INSTRUCTION(I64LessThanUnsigned, "i64.lt_u")
        else INSTRUCTION(I64LessEqualUnsigned, "i64.le_u")
        else INSTRUCTION(I64GreaterThanSigned, "i64.gt_s")
        else INSTRUCTION(I64GreaterEqualSigned, "i64.ge_s")
        else INSTRUCTION(I64GreaterThanUnsigned, "i64.gt_u")
        else INSTRUCTION(I64GreaterEqualUnsigned, "i64.ge_u")
        else INSTRUCTION(I64CountLeadingZeroes, "i64.clz")
        else INSTRUCTION(I64CountTrailingZeroes, "i64.ctz")
        else INSTRUCTION(I64PopulationCount, "i64.popcnt")

        else INSTRUCTION(AddressOf, "address_of")

        else INSTRUCTION(If, "if")
        else INSTRUCTION(IfElse, "if_else")
        else INSTRUCTION(Return, "return")

        else INSTRUCTION(GrowMemory, "grow_memory")
        else INSTRUCTION(PageSize, "page_size")
        else INSTRUCTION(CurrentMemory, "current_memory")

        else INSTRUCTION(I32Wrap, "i32.wrap/i64")
        else INSTRUCTION(I32TruncSignedF32, "i32.trunc_s/f32")
        else INSTRUCTION(I32TruncSignedF64, "i32.trunc_s/f64")
        else INSTRUCTION(I32TruncUnsignedF32, "i32.trunc_u/f32")
        else INSTRUCTION(I32TruncUnsignedF64, "i32.trunc_u/f64")
        else INSTRUCTION(I32ReinterpretF32, "i32.reinterpret/f32")

        else INSTRUCTION(I64ExtendSignedI32, "i64.extend_s/i32")
        else INSTRUCTION(I64ExtendUnsignedI32, "i64.extend_u/i32")
        else INSTRUCTION(I64TruncSignedF32, "i64.trunc_s/f32")
        else INSTRUCTION(I64TruncSignedF64, "i64.trunc_s/f64")
        else INSTRUCTION(I64TruncUnsignedF32, "i64.trunc_u/f32")
        else INSTRUCTION(I64TruncUnsignedF64, "i64.trunc_u/f64")
        else INSTRUCTION(I64ReinterpretF64, "i64.reinterpret/f64")

        else INSTRUCTION(F32DemoteF64, "f32.demote/f64")
        else INSTRUCTION(F32ConvertSignedI32, "f32.convert_s/i32")
        else INSTRUCTION(F32ConvertSignedI64, "f32.convert_s/i64")
        else INSTRUCTION(F32ConvertUnsignedI32, "f32.convert_u/i32")
        else INSTRUCTION(F32ConvertUnsignedI64, "f32.convert_u/i64")
        else INSTRUCTION(F32ReinterpretI32, "f32.reinterpret/i32")

        else INSTRUCTION(F64PromoteF32, "f64.promote/f32")
        else INSTRUCTION(F64ConvertSignedI32, "f64.convert_s/i32")
        else INSTRUCTION(F64ConvertSignedI64, "f64.convert_s/i64")
        else INSTRUCTION(F64ConvertUnsignedI32, "f64.convert_u/i32")
        else INSTRUCTION(F64ConvertUnsignedI64, "f64.convert_u/i64")
        else INSTRUCTION(F64ReinterpretI64, "f64.reinterpret/i64")

        else INSTRUCTION(Select, "select")

        else INSTRUCTION(F32Add, "f32.add")
        else INSTRUCTION(F32Sub, "f32.sub")
        else INSTRUCTION(F32Mul, "f32.mul")
        else INSTRUCTION(F32Div, "f32.div")
        else INSTRUCTION(F32Abs, "f32.abs")
        else INSTRUCTION(F32Neg, "f32.neg")
        else INSTRUCTION(F32CopySign, "f32.copysign")
        else INSTRUCTION(F32Ceil, "f32.ceil")
        else INSTRUCTION(F32Floor, "f32.floor")
        else INSTRUCTION(F32Trunc, "f32.trunc")
        else INSTRUCTION(F32Nearest, "f32.nearest")
        else INSTRUCTION(F32Equal, "f32.eq")
        else INSTRUCTION(F32NotEqual, "f32.ne")
        else INSTRUCTION(F32LesserThan, "f32.lt")
        else INSTRUCTION(F32LesserEqual, "f32.le")
        else INSTRUCTION(F32GreaterThan, "f32.gt")
        else INSTRUCTION(F32GreaterEqual, "f32.ge")
        else INSTRUCTION(F32Sqrt, "f32.sqrt")
        else INSTRUCTION(F32Min, "f32.min")
        else INSTRUCTION(F32Max, "f32.max")

        else INSTRUCTION(F64Add, "f64.add")
        else INSTRUCTION(F64Sub, "f64.sub")
        else INSTRUCTION(F64Mul, "f64.mul")
        else INSTRUCTION(F64Div, "f64.div")
        else INSTRUCTION(F64Abs, "f64.abs")
        else INSTRUCTION(F64Neg, "f64.neg")
        else INSTRUCTION(F64CopySign, "f64.copysign")
        else INSTRUCTION(F64Ceil, "f64.ceil")
        else INSTRUCTION(F64Floor, "f64.floor")
        else INSTRUCTION(F64Trunc, "f64.trunc")
        else INSTRUCTION(F64Nearest, "f64.nearest")
        else INSTRUCTION(F64Equal, "f64.eq")
        else INSTRUCTION(F64NotEqual, "f64.ne")
        else INSTRUCTION(F64LesserThan, "f64.lt")
        else INSTRUCTION(F64LesserEqual, "f64.le")
        else INSTRUCTION(F64GreaterThan, "f64.gt")
        else INSTRUCTION(F64GreaterEqual, "f64.ge")
        else INSTRUCTION(F64Sqrt, "f64.sqrt")
        else INSTRUCTION(F64Min, "f64.min")
        else INSTRUCTION(F64Max, "f64.max")
        else INSTRUCTION(Unreachable, "unreachable")
        else INSTRUCTION(Nop, "nop")
        else {
            return nullptr;
        }


    }
}
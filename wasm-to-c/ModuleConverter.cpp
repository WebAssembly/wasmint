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


#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float64.h>
#include <types/Float32.h>
#include "ModuleConverter.h"
#include "FunctionConverter.h"
#include <algorithm>
#include <iostream>
#include <instructions/Instructions.h>
#include <assert.h>

#define WASMINT_STRINGIFY_LIBWASM
#include "InjectedWasmLib.h"

void ModuleConverter::convert() {
    cSource_ << "#include <stdint.h>\n"
                "#include <stdio.h>\n"
                "#include <string.h>\n"
                "#include <stdlib.h>\n"
                "#include <math.h>\n"
                "#include <fenv.h>\n"
                "#include <malloc.h>\n";
    cSource_ << libWasmSource;
    cSource_ << "\n";
    cSource_ << "// ***********************************\n";
    cSource_ << "// *** Function declarations START ***\n";
    cSource_ << "// ***********************************\n";
    for (const wasm_module::Function* function : module_->functions()) {
        appendFunctionDeclaration(function);
        cSource_ << ";\n";
    }
    cSource_ << "// ***********************************\n";
    cSource_ << "// *** Function declarations  END  ***\n";
    cSource_ << "// ***********************************\n\n";

    cSource_ << "// ***********************************\n";
    cSource_ << "// *** Function definitions  START ***\n";
    cSource_ << "// ***********************************\n";
    for (const wasm_module::Function* function : module_->functions()) {
        appendFunctionDefinition(function);
        cSource_ << "\n\n";
    }
    cSource_ << "// ***********************************\n";
    cSource_ << "// *** Function definitions   END  ***\n";
    cSource_ << "// ***********************************\n\n";

    cSource_ << "int main() {\n";

    indent(4);
    cSource_ << "_wasm_init(" << module_->heapData().startSize() << ");\n";

    indent(4);

    for (const wasm_module::Function* function : module_->functions()) {
        if (function->name() == "main") {
            cSource_ << functionName(function) << "();\n";
        }
    }

    indent(4);
    cSource_ << "_wasm_deinit();\n";
    cSource_ << "}\n";
}

void ModuleConverter::convertFunction(const wasm_module::Function* function) {

}

void ModuleConverter::appendFunctionDeclaration(const wasm_module::Function* function) {
    cSource_ << toCType(function->returnType()) << " " << functionName(function) << "(";
    std::size_t i = 0;
    for (const wasm_module::Type* parameter : function->parameters()) {
        if (i != 0)
            cSource_ << ", ";
        cSource_ << toCType(parameter) << " " << variableName(function, i);
        i++;
    }
    cSource_ << ")";
}


std::string ModuleConverter::toCType(const wasm_module::Type* type) {
    if (type == wasm_module::Int32::instance()) {
        return "uint32_t";
    } else if (type == wasm_module::Int64::instance()) {
        return "uint64_t";
    } else if (type == wasm_module::Float32::instance()) {
        return "float";
    } else if (type == wasm_module::Float64::instance()) {
        return "double";
    } else if (type == wasm_module::Void::instance()) {
        return "void";
    } else {
        throw std::domain_error("toCType can't handle type " + type->name());
    }
}

std::string ModuleConverter::functionName(const wasm_module::FunctionSignature* function) {
    std::string result = "wasm__" + function->moduleName() + "_" + function->name();

    for (std::size_t i = 0; i < result.size(); i++) {
        char c = result[i];
        if (!std::isalnum(c)) {
            result[i] = '_';
        }
    }

    return result;
}

std::string ModuleConverter::variableName(const wasm_module::Function* function, std::size_t variableIndex) {
    return "var" + std::to_string(variableIndex);
}

void ModuleConverter::appendFunctionDefinition(const wasm_module::Function* function) {
    appendFunctionDeclaration(function);
    cSource_ << " {\n";

    std::size_t localIndex = function->parameters().size();
    for (const wasm_module::Type* localType : function->pureLocals()) {
        indent(4);
        cSource_ << toCType(localType) << " " << variableName(function, localIndex) << ";\n";
        localIndex++;
    }


    functionConverter = FunctionConverter();
    functionConverter.setFunction(function);
    function->mainInstruction()->foreachChild([&](const wasm_module::Instruction* instr){
        if (instr->returnType() != wasm_module::Void::instance()) {
            functionConverter.addInstruction(instr);
            indent(4);
            cSource_ << toCType(instr->returnType()) << " " << functionConverter(instr) << ";\n";
        }
        functionConverter.tryCreateLabel(instr);
    });

    for (auto& pair : functionConverter.floatLiteralVariables()) {
        indent(4);
        if (pair.first->id() == InstructionId::F32Const) {
            cSource_ << "uint32_t " << pair.second << " = " <<
            dynamic_cast<const wasm_module::Literal*>(pair.first)->literalValue().uint32Reinterpret() << "u;\n";
        } else if (pair.first->id() == InstructionId::F64Const) {
            cSource_ << "uint64_t " << pair.second << " = " <<
                    dynamic_cast<const wasm_module::Literal*>(pair.first)->literalValue().uint64Reinterpret() << "u;\n";
        } else {
            assert(false);
        }
    }

    serializeInstruction(*function->mainInstruction(), 4);

    if (function->returnType() != wasm_module::Void::instance()
        && function->mainInstruction()->returnType() != wasm_module::Void::instance()) {
        indent(4);
        cSource_ << "return " << functionConverter(function->mainInstruction()) << ";\n";
    }

    cSource_ << "}";
}

void ModuleConverter::serializeInstruction(const wasm_module::Instruction& instruction, std::size_t indentation) {

    switch (instruction.id()) {
        case InstructionId::I32Add:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " + " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;

        case InstructionId::I32Sub:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " - " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32Mul:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " * " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32DivSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = ((uint32_t)(" <<
                functionConverter(instruction.children().at(0), "int32_t") << " / " <<
                functionConverter(instruction.children().at(1), "int32_t") << "));\n";
            break;
        case InstructionId::I32DivUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " / " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32RemainderSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = ((uint32_t)(" <<
                functionConverter(instruction.children().at(0), "int32_t") << " % " <<
                functionConverter(instruction.children().at(1), "int32_t") << " < 0 ? -" <<
                functionConverter(instruction.children().at(1), "int32_t") << " : " <<
                functionConverter(instruction.children().at(1), "int32_t")
                << "));\n";
            break;
        case InstructionId::I32RemainderUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " % " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32And:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " & " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32Or:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " | " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32Xor:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " ^ " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32ShiftLeft:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
                functionConverter(instruction.children().at(0)) << " << " <<
                functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32ShiftRightZeroes:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " >> (" <<
            functionConverter(instruction.children().at(1)) << " % 32);\n";
            break;
            break;
        case InstructionId::I32Equal:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " == " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32NotEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " != " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32LessThanSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int32_t") << " < " <<
            functionConverter(instruction.children().at(1), "int32_t") << ";\n";
            break;
        case InstructionId::I32LessEqualSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int32_t") << " <= " <<
            functionConverter(instruction.children().at(1), "int32_t") << ";\n";
            break;
        case InstructionId::I32LessThanUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " < " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32LessEqualUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " <= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32GreaterThanSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int32_t") << " > " <<
            functionConverter(instruction.children().at(1), "int32_t") << ";\n";
            break;
        case InstructionId::I32GreaterEqualSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int32_t") << " >= " <<
            functionConverter(instruction.children().at(1), "int32_t") << ";\n";
            break;
        case InstructionId::I32GreaterThanUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " > " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I32GreaterEqualUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " >= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64Add:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " + " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64Sub:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " - " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64Mul:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " * " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64DivSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = ((uint64_t)" <<
            functionConverter(instruction.children().at(0), "int64_t") << " / " <<
            functionConverter(instruction.children().at(1), "int64_t") << ");\n";
            break;
        case InstructionId::I64DivUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " / " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64RemainderSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = ((uint64_t)(" <<
            functionConverter(instruction.children().at(0), "int64_t") << " % " <<
            functionConverter(instruction.children().at(1), "int64_t") << "));\n";
            break;
        case InstructionId::I64RemainderUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " % " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64And:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " & " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64Or:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " | " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64Xor:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " ^ " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64ShiftLeft:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " << " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64ShiftRightZeroes:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " >> (" <<
            functionConverter(instruction.children().at(1)) << " % 32);\n";
            break;
        case InstructionId::I64Equal:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " == " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64NotEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " != " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64LessThanSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int64_t") << " < " <<
            functionConverter(instruction.children().at(1), "int64_t") << ";\n";
            break;
        case InstructionId::I64LessEqualSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int64_t") << " <= " <<
            functionConverter(instruction.children().at(1), "int64_t") << ";\n";
            break;
        case InstructionId::I64LessThanUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " < " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64LessEqualUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " <= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64GreaterThanSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int64_t") << " > " <<
            functionConverter(instruction.children().at(1), "int64_t") << ";\n";
            break;
        case InstructionId::I64GreaterEqualSigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0), "int64_t") << " >= " <<
            functionConverter(instruction.children().at(1), "int64_t") << ";\n";
            break;
        case InstructionId::I64GreaterThanUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " > " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::I64GreaterEqualUnsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " >= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32Add:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " + " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32Sub:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " - " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32Mul:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " * " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32Div:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " / " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32Abs:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = fabsf(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F32Neg:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = -(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F32CopySign:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = copysignf(" <<
            functionConverter(instruction.children().at(0)) << ", " <<
            functionConverter(instruction.children().at(1)) << ");\n";
            break;
        case InstructionId::F32Ceil:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = ceilf(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F32Floor:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = floorf(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F32Trunc:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = truncf(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F32Nearest:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = nearbyintf(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F32Equal:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " == " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32NotEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " != " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32LesserThan:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " < " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32LesserEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " <= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32GreaterThan:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " > " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32GreaterEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " >= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F32Min:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = fminf(" <<
            functionConverter(instruction.children().at(0)) << "," <<
            functionConverter(instruction.children().at(1)) << ");\n";
            break;
        case InstructionId::F32Max:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = fmaxf(" <<
            functionConverter(instruction.children().at(0)) << "," <<
            functionConverter(instruction.children().at(1)) << ");\n";
            break;
        case InstructionId::F32Sqrt:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = sqrtf(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F64Add:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " + " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64Sub:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " - " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64Mul:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " * " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64Div:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " / " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64Abs:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = fabs(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F64Neg:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = -(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F64CopySign:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = copysign(" <<
            functionConverter(instruction.children().at(0)) << ", " <<
            functionConverter(instruction.children().at(1)) << ");\n";
            break;
        case InstructionId::F64Ceil:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = ceil(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F64Floor:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = floor(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F64Trunc:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = trunc(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F64Nearest:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = nearbyint(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;
        case InstructionId::F64Equal:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " == " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64NotEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " != " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64LesserThan:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " < " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64LesserEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " <= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64GreaterThan:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " > " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64GreaterEqual:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " <<
            functionConverter(instruction.children().at(0)) << " >= " <<
            functionConverter(instruction.children().at(1)) << ";\n";
            break;
        case InstructionId::F64Min:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = fmin(" <<
            functionConverter(instruction.children().at(0)) << "," <<
            functionConverter(instruction.children().at(1)) << ");\n";
            break;
        case InstructionId::F64Max:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = fmax(" <<
            functionConverter(instruction.children().at(0)) << "," <<
            functionConverter(instruction.children().at(1)) << ");\n";
            break;
        case InstructionId::F64Sqrt:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = sqrt(" <<
            functionConverter(instruction.children().at(0)) << ");\n";
            break;

        case InstructionId::I32Store8:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 1), &" << functionConverter(instruction.children().at(1)) << ", 1);\n";
            break;
        case InstructionId::I32Store16:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 2), &" << functionConverter(instruction.children().at(1)) << ", 2);\n";
            break;
        case InstructionId::I32Store:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 4), &" << functionConverter(instruction.children().at(1)) << ", 4);\n";
            break;
        case InstructionId::I64Store8:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 1), &" << functionConverter(instruction.children().at(1)) << ", 1);\n";
            break;
        case InstructionId::I64Store16:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 2), &" << functionConverter(instruction.children().at(1)) << ", 2);\n";
            break;
        case InstructionId::I64Store32:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 4), &" << functionConverter(instruction.children().at(1)) << ", 4);\n";
            break;
        case InstructionId::I64Store:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 8), &" << functionConverter(instruction.children().at(1)) << ", 8);\n";
            break;
        case InstructionId::F32Store:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 4), &" << functionConverter(instruction.children().at(1)) << ", 4);\n";
            break;
        case InstructionId::F64Store:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            indent(indentation);
            cSource_ << "memcpy(_wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 8), &" << functionConverter(instruction.children().at(1)) << ", 8);\n";
            break;

        case InstructionId::I32CountLeadingZeroes:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::I32CountTrailingZeroes:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::I32PopulationCount:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::I64CountLeadingZeroes:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::I64CountTrailingZeroes:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::I64PopulationCount:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::I32Const:
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " << dynamic_cast<const wasm_module::Literal&>(instruction).literalValue().uint32() << "u;\n";
            break;
        case InstructionId::I64Const:
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " << dynamic_cast<const wasm_module::Literal&>(instruction).literalValue().uint64() << "u;\n";
            break;
        case InstructionId::F32Const:
            indent(indentation);
            cSource_ << "memcpy(&" <<functionConverter(&instruction) << ", &" << functionConverter.getFloatLiteralVariable(&instruction) << ", 4);\n";
            break;
        case InstructionId::F64Const:
            indent(indentation);
            cSource_ << "memcpy(&" <<functionConverter(&instruction) << ", &" << functionConverter.getFloatLiteralVariable(&instruction) << ", 8);\n";
            break;
        case InstructionId::GetLocal:
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " << variableName(functionConverter.function(), dynamic_cast<const wasm_module::GetLocal&>(instruction).localIndex) << ";\n";
            break;
        case InstructionId::SetLocal:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << variableName(functionConverter.function(), dynamic_cast<const wasm_module::SetLocal&>(instruction).localIndex)
            << " = " << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::If:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "if (" << functionConverter(instruction.children().at(0)) << ") {\n";
            serializeInstruction(*instruction.children().at(1), indentation + 4);
            indent(indentation);
            cSource_ << "}\n";
            break;
        case InstructionId::IfElse:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "if (" << functionConverter(instruction.children().at(0)) << ") {\n";
            serializeInstruction(*instruction.children().at(1), indentation + 4);
            if (instruction.returnType() != wasm_module::Void::instance()) {
                indent(indentation + 4);
                cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(1)) << ";\n";
            }
            indent(indentation);
            cSource_ << "} else {\n";
            serializeInstruction(*instruction.children().at(2), indentation + 4);
            if (instruction.returnType() != wasm_module::Void::instance()) {
                indent(indentation + 4);
                cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(1)) << ";\n";
            }
            indent(indentation);
            cSource_ << "}\n";
            break;
        case InstructionId::Loop:
            indent(indentation);
            cSource_ << functionConverter.getLabel(&instruction) << "_1:;\n";

            for (const wasm_module::Instruction* child : instruction.children()) {
                serializeInstruction(*child, indentation);
            }
            indent(indentation);
            cSource_ << functionConverter.getLabel(&instruction) << "_0:;\n";
            break;

        case InstructionId::Label:
            for (const wasm_module::Instruction* child : instruction.children()) {
                serializeInstruction(*child, indentation);
            }
            indent(indentation);
            cSource_ << functionConverter.getLabel(&instruction) << "_0:;\n";
            break;
        case InstructionId::Block:
            for (const wasm_module::Instruction* child : instruction.children()) {
                serializeInstruction(*child, indentation);
            }
            indent(indentation);
            cSource_ << functionConverter.getLabel(&instruction) << "_0:;\n";
            break;
        case InstructionId::Nop:
            break;
        case InstructionId::Unreachable:
            indent(indentation);
            cSource_ << "fprintf(stderr, \"Encountered unreachable in %s line %d\\n\", __FILE__, __LINE__);\n";
            indent(indentation);
            cSource_ << "abort();\n";
            break;
        case InstructionId::Call:
        {
            for (const wasm_module::Instruction* child : instruction.children()) {
                serializeInstruction(*child, indentation);
            }
            indent(indentation);

            const wasm_module::FunctionSignature& targetFunction = dynamic_cast<const wasm_module::Call&>(instruction).functionSignature;

            if (targetFunction.returnType() != wasm_module::Void::instance())
                cSource_ << functionConverter(&instruction) << " = ";

            cSource_ << functionName(&targetFunction) << "(";
            std::size_t functionCallIndex = 0;
            for (const wasm_module::Instruction* child : instruction.children()) {
                if (functionCallIndex != 0)
                    cSource_ << ", ";
                cSource_ << functionConverter(child);
                functionCallIndex++;
            }
            cSource_ << ");\n";

            break;
        }
        case InstructionId::I32Select:
        case InstructionId::I64Select:
        case InstructionId::F32Select:
        case InstructionId::F64Select:
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            serializeInstruction(*instruction.children().at(2), indentation);
            indent(indentation);
            cSource_ << "if (" << functionConverter(instruction.children().at(0)) << ") {\n";
            if (instruction.returnType() != wasm_module::Void::instance()) {
                indent(indentation + 4);
                cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(1)) << ";\n";
            }
            indent(indentation);
            cSource_ << "} else {\n";
            if (instruction.returnType() != wasm_module::Void::instance()) {
                indent(indentation + 4);
                cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(2)) << ";\n";
            }
            indent(indentation);
            cSource_ << "}\n";
            break;

            
        case InstructionId::GrowMemory:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "_wasm_grow_heap(" << functionConverter(instruction.children().at(0)) << ");\n";
            break;

        case InstructionId::PageSize:
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = _wasm_pagesize;\n";
            break;

        case InstructionId::MemorySize:
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = _wasm_memorysize;\n";
            break;

        case InstructionId::Return:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "return " << functionConverter(instruction.children().at(0)) << ";\n";
            break;


        case InstructionId::Branch:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            const wasm_module::Branch& branch = dynamic_cast<const wasm_module::Branch&>(instruction);

            if (branch.returnType() != wasm_module::Void::instance()) {
                indent(indentation);
                cSource_ << functionConverter(branch.branchInformation()->target()) << " = "
                << functionConverter(instruction.children().at(0)) << ";\n";
            }
            indent(indentation);
            cSource_ << "goto " << functionConverter.getLabel(&branch.getBranchTarget()) << "_"
                << std::to_string(branch.branchInformation()->labelIndex()) << ";\n";
            break;
        }

        case InstructionId::BranchIf:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            serializeInstruction(*instruction.children().at(1), indentation);
            const wasm_module::BranchIf& branch = dynamic_cast<const wasm_module::BranchIf&>(instruction);

            indent(indentation);
            cSource_ << "if (" << functionConverter(instruction.children().at(0)) << ") {\n";
            indent(indentation + 4);

            if (branch.returnType() != wasm_module::Void::instance()) {
                indent(indentation);
                cSource_ << functionConverter(branch.branchInformation()->target()) << " = "
                << functionConverter(instruction.children().at(1)) << ";\n";
            }

            cSource_ << "goto " << functionConverter.getLabel(branch.branchInformation()->target()) << "_"
                << branch.branchInformation()->labelIndex() << ";\n";
            indent(indentation);
            cSource_ << "}\n";
            break;
        }
        case InstructionId::F64ReinterpretI64:
        case InstructionId::I64ReinterpretF64:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", &" << functionConverter(instruction.children().at(0)) << ", 8);\n";
            break;
        case InstructionId::I32Wrap:
        case InstructionId::F32ReinterpretI32:
        case InstructionId::I32ReinterpretF32:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", &" << functionConverter(instruction.children().at(0)) << ", 4);\n";
            break;
        case InstructionId::F32DemoteF64:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (float) " << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::F64PromoteF32:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (double) " << functionConverter(instruction.children().at(0)) << ";\n";
            break;

        case InstructionId::I32Load:
        case InstructionId::F32Load:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 4), 4);\n";
            break;

        case InstructionId::I64Load:
        case InstructionId::F64Load:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 8), 8);\n";
            break;

        case InstructionId::I32Load8Unsigned:
        case InstructionId::I64Load8Unsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 1), 1);\n";
            break;

        case InstructionId::I32Load16Unsigned:
        case InstructionId::I64Load16Unsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 2), 2);\n";
            break;

        case InstructionId::I64Load32Unsigned:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 4), 4);\n";
            break;
        case InstructionId::I32TruncSignedF32:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << " >= 2.14748365e+09f)\n";
            appendTrap(instruction, ">= 2.14748365e+09f", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " < INT32_MIN)\n";
            appendTrap(instruction, "< INT32_MIN", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value) ", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value) ", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (int32_t)" << valueVar << ";\n";
            break;
        }
        case InstructionId::I32TruncSignedF64:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << " > 2147483647.0)\n";
            appendTrap(instruction, "> 2147483647.0", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " < -2147483648.0)\n";
            appendTrap(instruction, "< -2147483648.0", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value)", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value)", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (int32_t)" << valueVar << ";\n";
            break;
        }
        case InstructionId::I32TruncUnsignedF32:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << " >= 4.2949673e+09f)\n";
            appendTrap(instruction, ">= 4.2949673e+09f", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " <= -1.0f)\n";
            appendTrap(instruction, "<= -1.0f", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value)", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value)", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (uint32_t)" << valueVar << ";\n";
            break;
        }
        case InstructionId::I32TruncUnsignedF64:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << " >= 4294967296)\n";
            appendTrap(instruction, ">= 4294967296", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " <= -1.0)\n";
            appendTrap(instruction, "<= -1.0", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value)", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value)", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (uint32_t)" << valueVar << ";\n";
            break;
        }
        case InstructionId::I64TruncSignedF32:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << ">= 9.22337204e+18f)\n";
            appendTrap(instruction, ">= 9.22337204e+18f", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " < -9223372036854775808.0f)\n";
            appendTrap(instruction, "< -9223372036854775808.0f", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value)", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value)", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (int64_t)" << valueVar << ";\n";
            break;
        }
        case InstructionId::I64TruncSignedF64:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << ">= 9.2233720368547758e+18)\n";
            appendTrap(instruction, ">= 9.2233720368547758e+18", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " < -9223372036854775808.0)\n";
            appendTrap(instruction, "< -9223372036854775808.0", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value)", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value)", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (int64_t)" << valueVar << ";\n";
            break;
        }
        case InstructionId::I64TruncUnsignedF32:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << ">= 1.84467441e+19f)\n";
            appendTrap(instruction, ">= 1.84467441e+19f", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " <= -1.0f)\n";
            appendTrap(instruction, "<= -1.0f", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value)", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value)", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (uint64_t)" << valueVar << ";\n";
            break;
        }
        case InstructionId::I64TruncUnsignedF64:
        {
            serializeInstruction(*instruction.children().at(0), indentation);
            std::string valueVar = functionConverter(instruction.children().at(0));

            indent(indentation);
            cSource_ << "if(" << valueVar << ">= 1.8446744073709552e+19)\n";
            appendTrap(instruction, ">= 1.8446744073709552e+19", indentation + 4);

            indent(indentation);
            cSource_ << "if(" << valueVar << " <= -1.0)\n";
            appendTrap(instruction, "<= -1.0", indentation + 4);

            indent(indentation);
            cSource_ << "if(isinf(" << valueVar << "))\n";
            appendTrap(instruction, "isinf(value)", indentation + 4);

            indent(indentation);
            cSource_ << "if(isnan(" << valueVar << "))\n";
            appendTrap(instruction, "isnan(value)", indentation + 4);

            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (uint64_t)" << valueVar << ";\n";
            break;
        }

        case InstructionId::I64ExtendSignedI32:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = ((uint64_t)((int64_t)((int32_t) " << functionConverter(instruction.children().at(0)) << ")));\n";
            break;
        case InstructionId::I64ExtendUnsignedI32:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = (uint64_t) " << functionConverter(instruction.children().at(0)) << ";\n";
            break;
        case InstructionId::F32ConvertSignedI32:
        case InstructionId::F64ConvertSignedI32:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(0), "int32_t") << ";\n";
            break;
        case InstructionId::F32ConvertSignedI64:
        case InstructionId::F64ConvertSignedI64:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(0), "int64_t") << ";\n";
            break;
        case InstructionId::F32ConvertUnsignedI32:
        case InstructionId::F32ConvertUnsignedI64:
        case InstructionId::F64ConvertUnsignedI32:
        case InstructionId::F64ConvertUnsignedI64:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(0)) << ";\n";
            break;

        case InstructionId::I32Load8Signed:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 1), 1);\n";
            indent(indentation);
            cSource_ << "if (" << functionConverter(&instruction) << " >= 128u)\n";
            indent(indentation);
            cSource_ << "    " << functionConverter(&instruction) << " |= 0xFFFFFF00;\n";
            break;
        case InstructionId::I32Load16Signed:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 2), 2);\n";
            indent(indentation);
            cSource_ << "if (" << functionConverter(&instruction) << " >= 32768u)\n";
            indent(indentation);
            cSource_ << "    " << functionConverter(&instruction) << " |= 0xFFFF0000;\n";
            break;
        case InstructionId::I64Load8Signed:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 1), 1);\n";
            indent(indentation);
            cSource_ << "if (" << functionConverter(&instruction) << " >= 128u)\n";
            indent(indentation);
            cSource_ << "    " << functionConverter(&instruction) << " |= 0xFFFFFFFFFFFFFF00;\n";
            break;
        case InstructionId::I64Load16Signed:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 2), 2);\n";
            indent(indentation);
            cSource_ << "if (" << functionConverter(&instruction) << " >= 32768u)\n";
            indent(indentation);
            cSource_ << "    " << functionConverter(&instruction) << " |= 0xFFFFFFFFFFFF0000;\n";
            break;
        case InstructionId::I64Load32Signed:
            serializeInstruction(*instruction.children().at(0), indentation);
            indent(indentation);
            cSource_ << functionConverter(&instruction) << " = 0;\n";
            indent(indentation);
            cSource_ << "memcpy(&" << functionConverter(&instruction) << ", _wasm_get_heap("
            << dynamic_cast<const wasm_module::LoadStoreInstruction*>(&instruction)->offset() << "u, "
            << functionConverter(instruction.children().at(0)) << ", 4), 4);\n";
            indent(indentation);
            cSource_ << "if (" << functionConverter(&instruction) << " >= 2147483648u)\n";
            indent(indentation);
            cSource_ << "    " << functionConverter(&instruction) << " |= 0xFFFFFFFF00000000;\n";
            break;

        case InstructionId::HasFeature: {
            const wasm_module::HasFeature* hasFeature = dynamic_cast<const wasm_module::HasFeature*>(&instruction);
            indent(indentation);
            cSource_ << functionConverter(&instruction);
            if (hasFeature->featureName() == "wasm") {
                cSource_ << " = 1;\n";
            } else {
                cSource_ << " = 0;\n";
            }
            break;
        }
        case InstructionId::I32ShiftRightSigned:
            indent(indentation);
            cSource_ << "{\n";
            indent(indentation + 4);
            cSource_ << functionConverter(instruction.children().at(1)) << " %= 32;\n";
            indent(indentation + 4);
            cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(0))
                     << " >> " << functionConverter(instruction.children().at(1)) << ";\n";
            indent(indentation + 4);
            cSource_ << "if (((int32_t) " << functionConverter(instruction.children().at(0)) << ") < 0) {\n";
            indent(indentation + 4);
            cSource_ << "    if ((" << functionConverter(instruction.children().at(0)) << " & (0x1u << 31u)) != 0) {\n";
            indent(indentation + 4);
            cSource_ << "        uint32_t i;\n";
            indent(indentation + 4);
            cSource_ << "        uint32_t bitMask = 0;\n";
            indent(indentation + 4);
            cSource_ << "        for (i = 0; i < " << functionConverter(instruction.children().at(1)) << "; i++) {\n";
            indent(indentation + 4);
            cSource_ << "            bitMask >>= 1u;\n";
            indent(indentation + 4);
            cSource_ << "            bitMask |= (0x1u << 31u);\n";
            indent(indentation + 4);
            cSource_ << "        }\n";
            indent(indentation + 4);
            cSource_ << "        " << functionConverter(&instruction) << " |= bitMask;\n";
            indent(indentation + 4);
            cSource_ << "    }\n";
            indent(indentation + 4);
            cSource_ << "}\n";

            indent(indentation);
            cSource_ << "}\n";
            break;
        case InstructionId::I64ShiftRightSigned:
            indent(indentation);
            cSource_ << "{\n";
            indent(indentation + 4);
            cSource_ << functionConverter(instruction.children().at(1)) << " %= 64;\n";
            indent(indentation + 4);
            cSource_ << functionConverter(&instruction) << " = " << functionConverter(instruction.children().at(0))
            << " >> " << functionConverter(instruction.children().at(1)) << ";\n";
            indent(indentation + 4);
            cSource_ << "if (((int64_t) " << functionConverter(instruction.children().at(0)) << ") < 0) {\n";
            indent(indentation + 4);
            cSource_ << "    if ((" << functionConverter(instruction.children().at(0)) << " & (0x1ul << 63u)) != 0) {\n";
            indent(indentation + 4);
            cSource_ << "        uint32_t i;\n";
            indent(indentation + 4);
            cSource_ << "        uint64_t bitMask = 0;\n";
            indent(indentation + 4);
            cSource_ << "        for (i = 0; i < " << functionConverter(instruction.children().at(1)) << "; i++) {\n";
            indent(indentation + 4);
            cSource_ << "            bitMask >>= 1u;\n";
            indent(indentation + 4);
            cSource_ << "            bitMask |= (0x1ul << 63u);\n";
            indent(indentation + 4);
            cSource_ << "        }\n";
            indent(indentation + 4);
            cSource_ << "        " << functionConverter(&instruction) << " |= bitMask;\n";
            indent(indentation + 4);
            cSource_ << "    }\n";
            indent(indentation + 4);
            cSource_ << "}\n";

            indent(indentation);
            cSource_ << "}\n";
            break;
        case InstructionId::CallImport:
        {
            for (const wasm_module::Instruction* child : instruction.children()) {
                serializeInstruction(*child, indentation);
            }
            indent(indentation);

            const wasm_module::FunctionSignature& targetFunction = dynamic_cast<const wasm_module::CallImport&>(instruction).functionSignature;

            if (targetFunction.returnType() != wasm_module::Void::instance())
                cSource_ << functionConverter(&instruction) << " = ";

            cSource_ << "_wasm_import_" << targetFunction.moduleName() << "_" << targetFunction.name() << "_";
            for (const wasm_module::Type* parameter : targetFunction.parameters()) {
                if (parameter == wasm_module::Int32::instance()) {
                    cSource_ << "i";
                }
                if (parameter == wasm_module::Int64::instance()) {
                    cSource_ << "l";
                }
                if (parameter == wasm_module::Float32::instance()) {
                    cSource_ << "f";
                }
                if (parameter == wasm_module::Float64::instance()) {
                    cSource_ << "d";
                }
            }

            cSource_ << "(";
            std::size_t functionCallIndex = 0;
            for (const wasm_module::Instruction* child : instruction.children()) {
                if (functionCallIndex != 0)
                    cSource_ << ", ";
                cSource_ << functionConverter(child);
                functionCallIndex++;
            }
            cSource_ << ");\n";

            break;
        }
        case InstructionId::CallIndirect:
        case InstructionId::TableSwitch:
        case InstructionId::Case:
        default:
            std::cerr << "Can't handle instruction " << instruction.name() << std::endl;
    }
}

void ModuleConverter::appendTrap(const wasm_module::Instruction& instruction, std::string reason, std::size_t indentation) {
    indent(indentation);
    cSource_ << "fprintf(stderr, \"Got trap in instruction " << instruction.name() << " (" << reason << ")\");\n";
    indent(indentation);
    cSource_ << "abort();\n";
}
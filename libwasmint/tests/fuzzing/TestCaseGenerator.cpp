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


#include "TestCaseGenerator.h"
#include <vector>
#include <iostream>
#include <types/Type.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>

namespace wasmint {

    using namespace wasm_module;

    struct TestCaseInstruction {
        std::string name;
        const Type* returnType;
        std::vector<const Type*> children;
        bool special;
    public:
        TestCaseInstruction() {
        }
        TestCaseInstruction(const char* name, const Type* returnType, const std::vector<const Type*>& children, bool special = false)
                : name(name), returnType(returnType), children(children), special(special) {
        }
    };

    std::vector<TestCaseInstruction> testInstructions = {
            TestCaseInstruction("i32.add", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.sub", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.mul", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.div_s", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.div_u", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.rem_s", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.rem_u", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.and", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.or", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.xor", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.shl", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.shr_u", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.shr_s", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.eq", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.ne", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.lt_s", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.le_s", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.lt_u", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.le_u", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.gt_s", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.ge_s", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.gt_u", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.ge_u", Int32::instance(), {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.clz", Int32::instance(), {Int32::instance()}),
            TestCaseInstruction("i32.ctz", Int32::instance(), {Int32::instance()}),
            TestCaseInstruction("i32.popcnt", Int32::instance(), {Int32::instance()}),


            TestCaseInstruction("i64.add", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.sub", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.mul", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.div_s", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.div_u", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.rem_s", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.rem_u", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.and", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.or", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.xor", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.shl", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.shr_u", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.shr_s", Int64::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.eq", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.ne", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.lt_s", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.le_s", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.lt_u", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.le_u", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.gt_s", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.ge_s", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.gt_u", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.ge_u", Int32::instance(), {Int64::instance(), Int64::instance()}),
            TestCaseInstruction("i64.clz", Int64::instance(), {Int64::instance()}),
            TestCaseInstruction("i64.ctz", Int64::instance(), {Int64::instance()}),
            TestCaseInstruction("i64.popcnt", Int64::instance(), {Int64::instance()}),

            TestCaseInstruction("return", nullptr, {nullptr}),
            TestCaseInstruction("grow_memory", nullptr, {Int32::instance()}),
            TestCaseInstruction("page_size", Int32::instance(), {}),
            TestCaseInstruction("memory_size", Int32::instance(), {}),

            TestCaseInstruction("i32.load8_s", Int32::instance(), {Int32::instance()}),
            TestCaseInstruction("i32.load8_u", Int32::instance(), {Int32::instance()}),
            TestCaseInstruction("i32.load16_s", Int32::instance(), {Int32::instance()}),
            TestCaseInstruction("i32.load16_u", Int32::instance(), {Int32::instance()}),
            TestCaseInstruction("i32.load", Int32::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.load8_s", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.load8_u", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.load16_s", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.load16_u", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.load32_s", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.load32_u", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.load", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("f32.load", Float32::instance(), {Int32::instance()}),
            TestCaseInstruction("f64.load", Float64::instance(), {Int32::instance()}),

            TestCaseInstruction("i32.store8", nullptr, {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.store16", nullptr, {Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i32.store", nullptr, {Int32::instance(), Int32::instance()}),

            TestCaseInstruction("i64.store8", nullptr, {Int32::instance(), Int64::instance()}),
            TestCaseInstruction("i64.store16", nullptr, {Int32::instance(), Int64::instance()}),
            TestCaseInstruction("i64.store32", nullptr, {Int32::instance(), Int64::instance()}),
            TestCaseInstruction("i64.store", nullptr, {Int32::instance(), Int64::instance()}),

            TestCaseInstruction("f32.store", nullptr, {Int32::instance(), Float32::instance()}),
            TestCaseInstruction("f64.store", nullptr, {Int32::instance(), Float64::instance()}),

            TestCaseInstruction("i32.wrap/i64", Int32::instance(), {Int64::instance()}),
            TestCaseInstruction("i32.trunc_s/f32", Int32::instance(), {Float32::instance()}),
            TestCaseInstruction("i32.trunc_s/f64", Int32::instance(), {Float64::instance()}),
            TestCaseInstruction("i32.trunc_u/f32", Int32::instance(), {Float32::instance()}),
            TestCaseInstruction("i32.trunc_u/f64", Int32::instance(), {Float64::instance()}),
            TestCaseInstruction("i32.reinterpret/f32", Int32::instance(), {Float32::instance()}),

            TestCaseInstruction("i64.extend_s/i32", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.extend_u/i32", Int64::instance(), {Int32::instance()}),
            TestCaseInstruction("i64.trunc_s/f32", Int64::instance(), {Float32::instance()}),
            TestCaseInstruction("i64.trunc_s/f64", Int64::instance(), {Float64::instance()}),
            TestCaseInstruction("i64.trunc_u/f32", Int64::instance(), {Float32::instance()}),
            TestCaseInstruction("i64.trunc_u/f64", Int64::instance(), {Float64::instance()}),
            TestCaseInstruction("i64.reinterpret/f64", Int64::instance(), {Float64::instance()}),

            TestCaseInstruction("f32.demote/f64", Float32::instance(), {Float64::instance()}),
            TestCaseInstruction("f32.convert_s/i32", Float32::instance(), {Int32::instance()}),
            TestCaseInstruction("f32.convert_s/i64", Float32::instance(), {Int64::instance()}),
            TestCaseInstruction("f32.convert_u/i32", Float32::instance(), {Int32::instance()}),
            TestCaseInstruction("f32.convert_u/i64", Float32::instance(), {Int64::instance()}),
            TestCaseInstruction("f32.reinterpret/i32", Float32::instance(), {Int32::instance()}),

            TestCaseInstruction("f64.promote/f32", Float64::instance(), {Float32::instance()}),
            TestCaseInstruction("f64.convert_s/i32", Float64::instance(), {Int32::instance()}),
            TestCaseInstruction("f64.convert_s/i64", Float64::instance(), {Int64::instance()}),
            TestCaseInstruction("f64.convert_u/i32", Float64::instance(), {Int32::instance()}),
            TestCaseInstruction("f64.convert_u/i64", Float64::instance(), {Int64::instance()}),
            TestCaseInstruction("f64.reinterpret/i64", Float64::instance(), {Int64::instance()}),

            TestCaseInstruction("i32.select", Int32::instance(), {Int32::instance(), Int32::instance(), Int32::instance()}),
            TestCaseInstruction("i64.select", Int64::instance(), {Int32::instance(), Int64::instance(), Int64::instance()}),
            TestCaseInstruction("f32.select", Float32::instance(), {Int32::instance(), Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f64.select", Float64::instance(), {Int32::instance(), Float64::instance(), Float64::instance()}),

            TestCaseInstruction("f32.add", Float32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.sub", Float32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.mul", Float32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.div", Float32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.abs", Float32::instance(), {Float32::instance()}),
            TestCaseInstruction("f32.neg", Float32::instance(), {Float32::instance()}),
            TestCaseInstruction("f32.copysign", Float32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.ceil", Float32::instance(), {Float32::instance()}),
            TestCaseInstruction("f32.floor", Float32::instance(), {Float32::instance()}),
            TestCaseInstruction("f32.trunc", Float32::instance(), {Float32::instance()}),
            TestCaseInstruction("f32.nearest", Float32::instance(), {Float32::instance()}),
            TestCaseInstruction("f32.eq", Int32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.ne", Int32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.lt", Int32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.le", Int32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.gt", Int32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.ge", Int32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.sqrt", Float32::instance(), {Float32::instance()}),
            TestCaseInstruction("f32.min", Float32::instance(), {Float32::instance(), Float32::instance()}),
            TestCaseInstruction("f32.max", Float32::instance(), {Float32::instance(), Float32::instance()}),

            TestCaseInstruction("f64.add", Float64::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.sub", Float64::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.mul", Float64::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.div", Float64::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.abs", Float64::instance(), {Float64::instance()}),
            TestCaseInstruction("f64.neg", Float64::instance(), {Float64::instance()}),
            TestCaseInstruction("f64.copysign", Float64::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.ceil", Float64::instance(), {Float64::instance()}),
            TestCaseInstruction("f64.floor", Float64::instance(), {Float64::instance()}),
            TestCaseInstruction("f64.trunc", Float64::instance(), {Float64::instance()}),
            TestCaseInstruction("f64.nearest", Float64::instance(), {Float64::instance()}),
            TestCaseInstruction("f64.eq", Int32::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.ne", Int32::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.lt", Int32::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.le", Int32::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.gt", Int32::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.ge", Int32::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.sqrt", Float64::instance(), {Float64::instance()}),
            TestCaseInstruction("f64.min", Float64::instance(), {Float64::instance(), Float64::instance()}),
            TestCaseInstruction("f64.max", Float64::instance(), {Float64::instance(), Float64::instance()}),

            TestCaseInstruction("unreachable", nullptr, {}),
            TestCaseInstruction("nop", nullptr, {}),

            TestCaseInstruction("if", nullptr, {nullptr, nullptr}),
            TestCaseInstruction("if_else", nullptr, {nullptr, nullptr, nullptr}),

            TestCaseInstruction("set_local", nullptr, {nullptr}, true),
            TestCaseInstruction("get_local", nullptr, {}, true),
            TestCaseInstruction("block", nullptr, {}, true),
            TestCaseInstruction("loop", nullptr, {}, true),
            TestCaseInstruction("label", nullptr, {}, true),

            TestCaseInstruction("br", nullptr, {nullptr}, true),
            TestCaseInstruction("br_if", nullptr, {Int32::instance(), nullptr}, true),

            //TestCaseInstruction("has_feature", Int32::instance(), {}, true),

            TestCaseInstruction("call", nullptr, {}, true),
            TestCaseInstruction("call_indirect", nullptr, {}, true),
            TestCaseInstruction("call_import", nullptr, {}, true),

            TestCaseInstruction("i32.const", Int32::instance(), {}, true),
            TestCaseInstruction("i64.const", Int64::instance(), {}, true),
            TestCaseInstruction("f32.const", Float32::instance(), {}, true),
            TestCaseInstruction("f64.const", Float64::instance(), {}, true),
    };


    std::vector<TestCaseInstruction> leafTestInstructions = {
            TestCaseInstruction("i32.const", Int32::instance(), {}, true),
            TestCaseInstruction("i64.const", Int64::instance(), {}, true),
            TestCaseInstruction("f32.const", Float32::instance(), {}, true),
            TestCaseInstruction("f64.const", Float64::instance(), {}, true),
            TestCaseInstruction("nop", nullptr, {})
    };

    void TestCaseGenerator::generateMainFunction() {
        source_ << "(func main ";

        while (decision(90))
            generateInstruction();
        source_ << ")";
    }

    void TestCaseGenerator::generateRandomString() {

        std::size_t size = randomInRange(35);
        for (std::size_t i = 0; i < size; i++) {
            if (decision(99)) {
                char c = 0;
                if (decision(33)) {
                    c = (char) (randomInRange(26) + 'a');
                } else if (decision(50)) {
                    c = (char) (randomInRange(26) + 'A');
                } else {
                    c = (char) (randomInRange(10) + '0');
                }
                source_ << c;
            } else {
                source_ << ((char)randomInRange(255));
            }
        }
        source_ << " ";
    }

    void TestCaseGenerator::generateImports() {
    }
}


void wasmint::TestCaseGenerator::generateInstruction(const wasm_module::Type* expectedReturnType) {
    instructions++;
    TestCaseInstruction* instruction;
    if (instructions < instructionLimit) {
        while(true) {
            std::size_t index = randomInRange(testInstructions.size());
            instruction = &testInstructions.at(index);
            if (expectedReturnType == nullptr)
                break;
            if (instruction->returnType == expectedReturnType)
                break;
        }
    } else {
        if (expectedReturnType == Int32::instance()) {
            instruction = &leafTestInstructions.at(0);
        } else if (expectedReturnType == Int64::instance()) {
            instruction = &leafTestInstructions.at(1);
        } else if (expectedReturnType == Float32::instance()) {
            instruction = &leafTestInstructions.at(2);
        } else if (expectedReturnType == Float64::instance()) {
            instruction = &leafTestInstructions.at(3);
        } else {
            instruction = &leafTestInstructions.at(4);
        }
    }

    source_ << " (" << instruction->name << " ";

    if (instruction->special) {
        if (instruction->name == "i32.const") {
            source_ << randomInRange(1000);
        } else if (instruction->name == "i64.const") {
            source_ << randomInRange(1000);
        } else if (instruction->name == "f32.const") {
            source_ << randomInRange(1000);
        } else if (instruction->name == "f64.const") {
            source_ << randomInRange(1000);
        } else if (instruction->name == "has_feature") {
            generateRandomString();
        } else if (instruction->name == "get_local") {
            generateRandomString();
        } else if (instruction->name == "set_local") {
            generateRandomString();
        } else if (instruction->name == "label") {
            generateRandomString();
        } else if (instruction->name == "block") {
            generateRandomString();
        } else if (instruction->name == "loop") {
            generateRandomString();
        } else if (instruction->name == "call_import") {
            generateRandomString();
        } else if (instruction->name == "call") {
            generateRandomString();
        } else if (instruction->name == "call_indirect") {
            generateRandomString();
        } else if (instruction->name == "br") {
            generateRandomString();
        } else if (instruction->name == "br_if") {
            generateRandomString();
        } else {
            std::cerr << "Unknown special instruction during generation " << instruction->name << std::endl;
            exit(1);
        }
    }
    source_ << " ";

    if (instructions < instructionLimit) {
        for (std::size_t i = 0; i < instruction->children.size(); i++) {
            const Type *expectedChildType = nullptr;
            if (i < instruction->children.size()) {
                expectedChildType = instruction->children.at(i);
            }
            generateInstruction(expectedChildType);
        }
    }

    source_ << ") ";
}

void wasmint::TestCaseGenerator::generateFunction() {
    source_ << "(func ";
    generateRandomString();
    generateInstruction();
    source_ << ")";
}

void wasmint::TestCaseGenerator::mutateSource() {
    if (finalSource_.empty())
        return;
    std::size_t start = randomInRange(finalSource_.size());
    std::size_t length = randomInRange(finalSource_.size());
    finalSource_.erase(start, length);
}

void wasmint::TestCaseGenerator::generateMemory() {

}

void wasmint::TestCaseGenerator::generateModule() {
    source_ << "module ";
    if (decision(50))
        generateMemory();

    while (decision(90))
        generateFunction();

    if (decision(99))
        generateMainFunction();

    if (decision(3))
        mutateSource();
}
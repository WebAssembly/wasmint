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


#ifndef WASMINT_SPECTESTMODULE_H
#define WASMINT_SPECTESTMODULE_H

#include <iostream>

#include <Module.h>
#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>

namespace wasmint {
    class SpectestModule : public wasm_module::Module {

        static std::stringstream stdout_;
        uint32_t counter_ = 0;

    public:
        static wasm_module::Module* create() {
            using namespace wasm_module;
            SpectestModule* module = new SpectestModule();
            module->context().name("stdio");

            module->addVariadicFunction("print", Void::instance(),
            [](std::vector<Variable> parameters) {

                    for (const Variable& parameter : parameters) {
                        if (&parameter.type() == Int32::instance()) {
                            stdout_ << parameter.int32() << " : i32\n";
                        } else if (&parameter.type() == Int64::instance()) {
                            stdout_ << parameter.int64() << " : i64\n";
                        } else if (&parameter.type() == Float32::instance()) {
                            std::stringstream ss;
                            ss << parameter.float32();
                            std::string value = ss.str();
                            if (value.find(".") == std::string::npos) {
                                value += '.';
                            }
                            stdout_ << value << " : f32\n";
                        } else if (&parameter.type() == Float64::instance()) {
                            std::stringstream ss;
                            ss << parameter.float64();
                            std::string value = ss.str();
                            if (value.find(".") == std::string::npos) {
                                value += '.';
                            }
                            stdout_ << value << " : f64\n";
                        } else {
                            stdout_ << "Unknown type to print" << parameter.type().name();
                        }
                    }
                    return Void::instance();
                }
            );

            // alternating returns 1 or the sequence of increasing uneven numbers
            // only useful for testing the halting problem detector
            module->addFunction("uneven", Int32::instance(), {},
                [=](std::vector<Variable> parameters) {
                    module->counter_++;
                    if (module->counter_ % 2 == 0) {
                        return Variable::createInt32(1);
                    } else {
                        return Variable::createInt32(module->counter_);
                    }
                }
            );

            return module;
        }

#undef stdout
        static std::stringstream& stdout() {
            return stdout_;
        }
    };
}

#endif //WASMINT_SPECTESTMODULE_H

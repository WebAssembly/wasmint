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

#include <iostream>

#include <types/Int32.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>

#include "StdioModule.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace wasmint {

wasm_module::Module* StdioModule::create() {
    using namespace wasm_module;
    Module* module = new Module();
    module->context().name("stdio");

    module->addVariadicFunction("print", Void::instance(),
                                [](std::vector<Variable> parameters) {

                                        for (const Variable& parameter : parameters) {
                                            std::cout << "print ";

                                            if (&parameter.type() == Int32::instance()) {
                                                std::cout << parameter.int32();
                                            } else if (&parameter.type() == Int64::instance()) {
                                                std::cout << parameter.int64();
                                            } else if (&parameter.type() == Float32::instance()) {
                                                std::cout << parameter.float32();
                                            } else if (&parameter.type() == Float64::instance()) {
                                                std::cout << parameter.float64();
                                            } else {
                                                std::cout << "Unknown type to print" << parameter.type().name();
                                            }
                                            std::cout << std::endl;

                                        }

                                        return Void::instance();
                                    });

        module->addFunction("sleep", Void::instance(), {Int32::instance()},
                                    [](std::vector<Variable> parameters) {
#ifdef _WIN32
                                        Sleep(Int32::getValue(parameters.at(0)));
#else
                                        usleep(Int32::getValue(parameters.at(0)));
#endif
                                        return Void::instance();
                                    });
        return module;
}

}
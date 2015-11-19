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

#ifndef WASMINT_STDIOMODULE_H
#define WASMINT_STDIOMODULE_H


#include <Module.h>
#include <types/Int32.h>
#include <iostream>
#include <interpreter/Thread.h>
#include <unistd.h>

namespace wasmint {
    class StdioModule {

    public:
        static wasm_module::Module* create() {
            using namespace wasm_module;
            Module* module = new Module();
            module->context().name("stdio");

            module->addFunction("print", Void::instance(), {Int32::instance()},
                                        [](std::vector<Variable> parameters) {
                                            std::cout << "print_i32 " << Int32::getValue(parameters.at(0)) << std::endl;
                                            return Void::instance();
                                        });

            module->addFunction("sleep", Void::instance(), {Int32::instance()},
                                        [](std::vector<Variable> parameters) {
                                            usleep(Int32::getValue(parameters.at(0)));
                                            return Void::instance();
                                        });
            return module;
        }
    };
}

#endif //WASMINT_STDIOMODULE_H

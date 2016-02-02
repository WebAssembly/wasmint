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


#ifndef WASMINT_RANDOMMODULE_H
#define WASMINT_RANDOMMODULE_H



#include <Module.h>
#include <types/Int32.h>
#include <iostream>
#include <unistd.h>
#include <types/Int64.h>
#include <types/Float32.h>
#include <types/Float64.h>
#include <random>

namespace wasmint {
    class RandomModule : public wasm_module::Module {

        std::mt19937 generator;

    public:
        RandomModule() : generator(0) {

        }

        static wasm_module::Module* create() {
            using namespace wasm_module;
            RandomModule* module = new RandomModule();
            module->context().name("random");

            module->addFunction("rand32", Int32::instance(), {},
                [=](std::vector<Variable> parameters) {
                    return Variable::createInt32((int32_t) module->generator());
                }
            );
            return module;
        }
    };
}


#endif //WASMINT_RANDOMMODULE_H

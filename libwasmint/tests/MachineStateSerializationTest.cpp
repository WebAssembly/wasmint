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


#include <cstdint>
#include <Module.h>
#include <types/Int32.h>
#include <interpreter/at/MachineState.h>
#include <sexpr_parsing/CharacterStream.h>
#include <sexpr_parsing/SExprParser.h>
#include <sexpr_parsing/ModuleParser.h>

using namespace wasm_module;
using namespace wasm_module::sexpr;
using namespace wasmint;

int main() {
    std::vector<uint8_t> memory;

    std::unique_ptr<MachineState> environment;
    environment.reset(new MachineState());

    Module* module = ModuleParser::parse("module (func $main (if_else (i32.sub (i32.const 1) (i32.const 1)) (unreachable) (i32.add (i32.const 1) (i32.const 3))))");

    environment->useModule(*module, false);

    environment->createThread().startAtFunction(module->name(), "$main");

    InterpreterThread * thread = &environment->getThread();

    while (thread->canStep()) {
        thread->step();

        memory.clear();
        ByteOutputStream outputStream(memory);
        environment->serialize(outputStream);

        environment.reset(new MachineState());
        environment->useModule(*module, false);

        ByteInputStream inputStream(memory);
        environment->setState(inputStream);
        thread = &environment->getThread();
    }

    delete module;

}
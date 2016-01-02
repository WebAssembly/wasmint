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
#include <assert.h>
#include <interpreter/WasmintVM.h>
#include <iostream>

using namespace wasm_module;
using namespace wasm_module::sexpr;
using namespace wasmint;

int main() {
    WasmintVM vm;

    Module* module = ModuleParser::parse("module (memory 5 5) (func main (i32.store (i32.const 0) (i32.const 4)) (if_else (i32.add (i32.const 2) (i32.const 4)) (nop) (unreachable)))");
    vm.loadModule(*module, true);
    vm.startAtFunction(*module->functions().front());


    bool lastTest = false;
    for (std::size_t i = 0;;i++) {
        VMState backupState = vm.state();
        assert(vm.state() == backupState);
        for (std::size_t j = 0; j < i; j++) {
            vm.step();
            assert(vm.state() != backupState);

            if (vm.finished()) {
                lastTest = true;
            }
        }
        for (std::size_t j = 0; j < i; j++) {
            assert(vm.state() != backupState);
            vm.stepBack();
        }
        assert(vm.state() == backupState);

        if (lastTest) {
            break;
        }
    }

}
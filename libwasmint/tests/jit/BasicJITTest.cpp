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
    {
        WasmintVM vm;

        Module* module = ModuleParser::parse("module (func $main (if_else (i32.const 0) (unreachable) (nop)))");
        vm.loadModule(*module, true);
        vm.startAtFunction(*module->functions().front());

        if (vm.gotTrap()) {
            std::cerr << "Got trap: " << vm.trapReason() << std::endl;
        }
        assert(!vm.gotTrap());
    }
    {
        WasmintVM vm;

        Module* module = ModuleParser::parse("module (func $main (if_else (i32.const 1) (unreachable) (nop)))");
        vm.loadModule(*module, true);
        vm.startAtFunction(*module->functions().front());
        vm.stepUntilFinished();
        if (!vm.gotTrap()) {
            std::cerr << "Got no trap but shouldn't trap " << std::endl;
        }
        assert(vm.gotTrap());
    }

}
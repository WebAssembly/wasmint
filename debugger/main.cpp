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


#include <interpreter/heap/Heap.h>
#include <Module.h>
#include "WasmDbg.h"

int main(int argc, char** argv) {

    WasmDbg::initCurses();

    WasmDbg debugger;


    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg.find("--") == 0) {
            std::cerr << "Unknown argument " << arg << std::endl;
            return 2;
        } else {
            const std::string& modulePath = argv[i];

            try {
                debugger.vm().loadModule(modulePath);
            } catch (const std::exception& e) {
                std::cerr << "Got exception while parsing sexpr module "
                << modulePath << ": " << e.what() << " (typeid name " << typeid(e).name() << ")"
                << std::endl;
                return 1;
            }
        }
    }

    debugger.run();

    WasmDbg::deinitCurses();

}
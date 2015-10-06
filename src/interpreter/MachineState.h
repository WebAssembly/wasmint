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



#ifndef WASMINT_ENVIRONMENT_H
#define WASMINT_ENVIRONMENT_H


#include <Variable.h>
#include <vector>
#include <memory>
#include <stack>
#include <map>
#include <Module.h>
#include "Heap.h"

namespace wasmint {

    ExceptionMessage(NoFunctionWithName)

    ExceptionMessage(NoGlobalWithName)

    class CalledBreak;

    class CalledContinue;

    class Thread;

/**
 * Contains all variable values during the interpretation of a program.
 */
    class MachineState {

        /**
     * All functions that are accessible with the currently loaded modules.
     * The keys are the function names.
     */
        std::map<std::string, wasm_module::Function *> functions_;

        /**
     * All globals that are accessible with the currently loaded modules.
     * The keys are the variable names.
     */
        std::map<std::string, wasm_module::Variable> globals_;

        /**
     * The current heap.
     */
        Heap heap_;

        /**
     * The stdout of this program. We currently just append to this string and then read it via stdou().
     */
        std::string stdout_;

        // FIXME Use smart pointers if possible...
        std::vector<Thread *> threads_;

    public:
        MachineState() : heap_(1024) {
        }

        virtual ~MachineState();

        Thread &createThread();

        void useModule(wasm_module::Module &module);

        Heap &heap() {
            return heap_;
        }

        wasm_module::Variable &global(std::string name) {
            auto globalIterator = globals_.find(name);
            if (globalIterator != globals_.end()) {
                return globalIterator->second;
            } else {
                throw NoGlobalWithName(name);
            }
        }

        void print(std::string s) {
            stdout_ += s;
        }

        std::string stdout() {
            return stdout_;
        }

        std::map<std::string, wasm_module::Function *> &functions() {
            return functions_;
        }

        /**
     * All globals that are accessible with the currently loaded modules.
     * The keys are the variable names.
     */
        std::map<std::string, wasm_module::Variable> &globals() {
            return globals_;
        }
    };

}

#endif //WASMINT_ENVIRONMENT_H

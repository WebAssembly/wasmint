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
#include <serialization/ByteInputStream.h>
#include <serialization/ByteOutputStream.h>
#include <serialization/Serializeable.h>
#include "Heap.h"
#include "Thread.h"

namespace wasmint {

    ExceptionMessage(NoModuleWithName)

    ExceptionMessage(NoFunctionWithName)

    class Thread;

    class MachineState : public Serializeable {

        // FIXME Use smart pointers if possible...
        std::vector<Thread *> threads_;

        std::map<std::string, wasm_module::Module*> modules_;

        std::vector<wasm_module::Module*> modulesToDelete_;

    public:
        MachineState()  {
        }
        virtual ~MachineState();

        void operator=(const MachineState& other); // don't implement
        MachineState(const MachineState& other); // don't implement

        void setState(ByteInputStream& stream);

        // TODO function doesn't work for multiple threads
        Thread& getThread() {
            return *threads_.front();
        }

        void deleteThread() {
            delete threads_.front();
            threads_.clear();
        }

        Thread &createThread();

        void useModule(wasm_module::Module &module, bool takeMemoryOwnership = false);

        wasm_module::Module& getModule(const std::string& moduleName) {
            auto iter = modules_.find(moduleName);
            if (iter != modules_.end()) {
                return *iter->second;
            } else {
                throw NoModuleWithName(moduleName);
            }
        }

        wasm_module::Function& getFunction(const std::string& moduleName, const std::string& functionName) {
            return getModule(moduleName).getFunction(functionName);
        }

        virtual void serialize(ByteOutputStream& stream) const override;

        const wasm_module::Instruction* getInstruction(const wasm_module::InstructionAddress& address);
    };

}

#endif //WASMINT_ENVIRONMENT_H

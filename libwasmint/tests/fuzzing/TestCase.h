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


#ifndef WASMINT_TESTCASE_H
#define WASMINT_TESTCASE_H

#include <random>
#include <sstream>
#include <interpreter/WasmintVM.h>
#include <ModuleLoader.h>
#include <sexpr_serializing/ModuleSerializer.h>
#include "DataRescuer.h"
#include "TestCaseGenerator.h"


namespace wasmint {
    class TestCase {

        long long seed_;
        std::mt19937 randomGenerator;
        std::string sourceString_;

        bool passedParsing_ = false;
        bool passedRunning_ = false;

        TestCaseGenerator testCaseGenerator_;

        void generate() {
            testCaseGenerator_.generate();
            sourceString_ = testCaseGenerator_.source();
        }

        void testParsing() {
            wasm_module::Module* module = wasm_module::sexpr::ModuleParser::parse(sourceString_, "testModule");
            passedParsing_ = true;

            std::stringstream serializeOut;
            wasm_module::sexpr::ModuleSerializer::serialize(serializeOut, module);
            wasm_module::Module* testModule = wasm_module::sexpr::ModuleParser::parse(serializeOut.str(), module->name());
            if ((*testModule) == (*module)) {
            } else {
                std::cout << "Comparison failed" << std::endl;
                std::cout << serializeOut.str() << std::endl;
                std::cout << sourceString_ << std::endl;
                exit(1);
            }
            delete testModule;
        }

        void testRunning() {
            WasmintVM vm;

            vm.loadModuleFromData(sourceString_);
            const wasm_module::Module* module = vm.modules().back();
            vm.startAtFunction(*module->function("$main"));

            vm.stepUntilFinished(false);

            for (unsigned long i = 0; i < 1000000; i++) {
                if (vm.finished())
                    break;
                vm.step();
            }
            passedRunning_ = true;
        }

    public:
        TestCase(long long seed) : seed_(seed), randomGenerator(seed), testCaseGenerator_(seed) {

        }

        void run() {
            generate();
            sourceString_ = testCaseGenerator_.source();

            try {
                testParsing();
                testRunning();
            } catch (const std::exception& e) {
                //std::cout << typeid(e).name() << " : " << e.what() << std::endl;
            }
        }

        bool passedParsing() const {
            return passedParsing_;
        }

        bool passedRunning() const {
            return passedRunning_;
        }
    };
}



#endif //WASMINT_TESTCASE_H

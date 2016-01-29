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


#ifndef WASMINT_TESTCASEGENERATOR_H
#define WASMINT_TESTCASEGENERATOR_H

#include <string>
#include <sstream>
#include <random>
#include <types/Type.h>
#include <iostream>

namespace wasmint {
    class TestCaseGenerator {

        long long seed_;
        std::stringstream source_;
        std::mt19937 randomGenerator;
        std::string finalSource_;

        uint32_t instructions = 0;
        uint32_t instructionLimit = 50;

        unsigned long randomInRange(std::size_t range) {
            return randomGenerator() % range;
        }

        bool decision(int chance) {
            return randomInRange(100) < chance;
        }

        void generateRandomString();

        void generateImports();

        void generateModule();

        void generateMemory();

        void generateInstruction(const wasm_module::Type* expectedReturnType = nullptr);

        void generateFunction();

        void generateMainFunction();

        void mutateSource();

    public:
        TestCaseGenerator(long long seed) : seed_(seed), randomGenerator(seed) {
        }

        void generate() {
            generateModule();
            finalSource_ = source_.str();
            //if (decision(3))
            //    mutateSource();

            //std::cout << finalSource_ << std::endl;
        }

        const std::string& source() {
            return finalSource_;
        }
    };
}


#endif //WASMINT_TESTCASEGENERATOR_H

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


#ifndef WASMINT_GENERATIONFUZZERMAIN_H
#define WASMINT_GENERATIONFUZZERMAIN_H

#include <random>
#include <sstream>
#include <interpreter/WasmintVM.h>
#include <fstream>
#include "DataRescuer.h"
#include "TestCaseGenerator.h"
#include <ostream>
#include <iostream>

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    std::ostream& operator<<(std::ostream& os, Code code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }
}



namespace wasmint {

    class Fuzzer {

        static const std::string& lastSeedFile;

        long long startSeed_ = 0;
        long long currentSeed_ = 0;
        long long testsFailed_ = 0;

        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point lastReportTime;
        long long runtimeInSeconds_ = 0;
        long long reportIntervalInSeconds_ = 1;

        long long handledTestCases = 0;
        long long testCasesPassedParsing = 0;
        long long testCasesPassedRunning = 0;

        wasmint::DataRescuer rescuer;

        bool shutdown_ = false;

        void printReport();

        bool readLongLong(const std::string& filePath, long long& value) {
            bool result;

            std::ifstream inputFile;
            inputFile.open(filePath.c_str());
            result = static_cast<bool>(inputFile >> value);
            inputFile.close();
            return result;
        }

        void writeLongLong(const std::string& filePath, const long long& value) {
            std::ofstream outputFile;
            outputFile.open(filePath.c_str());

            outputFile << value;

            outputFile.close();
        }

        void saveCurrentSeed() {
            writeLongLong(lastSeedFile, currentSeed_);
        }

    public:
        Fuzzer();

        void run();

        void runTest(long long seed);

        void shutdown() {
            shutdown_ = true;
        }
    };
}
wasmint::Fuzzer* fuzzerInstance = nullptr;

inline void handleSigint(int signo) {
    fuzzerInstance->shutdown();
}

#endif //WASMINT_GENERATIONFUZZERMAIN_H

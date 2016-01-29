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


#include <chrono>
#include "Fuzzer.h"
#include "TestCase.h"


int main(int argc, char** argv) {
    wasmint::Fuzzer fuzzer;
    fuzzer.run();
}

const std::string& wasmint::Fuzzer::lastSeedFile = "lastFuzzerSeed";

void wasmint::Fuzzer::run() {
    startTime = std::chrono::steady_clock::now();

    for(long seed_ = startSeed_;; seed_++) {
        runTest(seed_);

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        long minutesSinceLastReport = std::chrono::duration_cast<std::chrono::seconds>(now - lastReportTime).count();
        if (minutesSinceLastReport >= reportIntervalInSeconds_) {
            printReport();
            lastReportTime = now;
        }

        if (runtimeInSeconds_ != 0) {
            long minutesSinceStart = std::chrono::duration_cast<std::chrono::seconds>(now - lastReportTime).count();
            if (minutesSinceStart > runtimeInSeconds_) {
                printReport();
                break;
            }
        }

        if (shutdown_)
            break;
        handledTestCases++;
    }
    printReport();
    saveCurrentSeed();
}

wasmint::Fuzzer::Fuzzer() {
    fuzzerInstance = this;

    std::cout << "Attaching handler to SIGINT for exiting fuzzer...   ";
    if (signal(SIGINT, &handleSigint) != SIG_ERR) {
        std::cout << "[" << Color::FG_GREEN << " OK " << Color::FG_DEFAULT << "]" << std::endl;
    } else {
        std::cout << "[" << Color::FG_RED << "FAIL" << Color::FG_DEFAULT << "]" << std::endl;
    }

    std::cout << "Attaching data rescuer to system calls...           ";
    wasmint::DataRescuer::attachHandler();
    std::cout << "[" << Color::FG_GREEN << " OK " << Color::FG_DEFAULT << "]" << std::endl;

    std::cout << "Looking for start seed in " << lastSeedFile << "...         ";
    if (readLongLong(lastSeedFile, startSeed_)) {
        std::cout << "[" << Color::FG_GREEN << " OK " << Color::FG_DEFAULT << "]" << std::endl;
        std::cout << "Using start seed: " << Color::FG_BLUE << startSeed_ << Color::FG_DEFAULT << std::endl;
    } else {
        std::cout << "[" << Color::FG_RED << "FAIL" << Color::FG_DEFAULT << "]" << std::endl;
    }
}

void wasmint::Fuzzer::printReport() {
    // remove newline
    std::cout << "\rSeed: " << Color::FG_BLUE << currentSeed_ << Color::FG_DEFAULT;
    std::cout << "| Performance: " << Color::FG_BLUE << (handledTestCases / reportIntervalInSeconds_) << Color::FG_DEFAULT << " Tests/s ";
    std::cout << "| Parsed: " << Color::FG_BLUE << (testCasesPassedParsing * 100.0 / handledTestCases) << "%  " << Color::FG_DEFAULT;
    std::cout << "| Run: " << Color::FG_BLUE << (testCasesPassedRunning * 100.0 / handledTestCases) << "%  " << Color::FG_DEFAULT;
    handledTestCases = 0;
    testCasesPassedParsing = 0;
    testCasesPassedRunning = 0;
    std::cout.flush();
}

void wasmint::Fuzzer::runTest(long long seed) {
    currentSeed_ = seed;
    TestCase testCase(seed);
    testCase.run();
    if (testCase.passedParsing()) {
        testCasesPassedParsing++;
    }
    if (testCase.passedRunning()) {
        testCasesPassedRunning++;
    }
}
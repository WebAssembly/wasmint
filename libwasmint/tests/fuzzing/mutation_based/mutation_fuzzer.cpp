
#include <random>
#include <interpreter/at/MachineState.h>
#include <iostream>
#include "../common/SExprExecutor.h"
#include "../common/DataRescuer.h"

using namespace wasm_module;
using namespace wasmint;

namespace wasmint {
    void fuzz(unsigned seed) {
        std::mt19937 randomGenerator(seed);
        std::size_t size = randomGenerator() % 10000u;

        std::string program;
        program.reserve(size);

        for (std::size_t i = 0; i < size; i++) {
            program.push_back((char) ((int) (randomGenerator() % 256) - 128));
        }

        SExprExecutor executor;
        executor.execute(program);
    }
}


int main(int argc, char** argv) {
    unsigned long tries = 3000;

    wasmint::DataRescuer::attachHandler();

    if (argc == 2) {
        tries = (unsigned long) std::atol(argv[1]);
    }

    for(unsigned seed = 0; seed < tries; seed++) {
        wasmint::DataRescuer rescuer(seed);
        wasmint::fuzz(seed);
    }
}
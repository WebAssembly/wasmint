
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <binary_parsing/ByteStream.h>
#include <Module.h>
#include <binary_parsing/ModuleParser.h>
#include <interpreter/at/MachineState.h>
#include <sexpr_parsing/ModuleParser.h>
#include <sexpr_parsing/SExprParser.h>
#include <builtins/StdioModule.h>
#include <interpreter/WasmintVM.h>
#include <interpreter/halting/HaltingProblemDetector.h>
#include <builtins/RandomModule.h>
#include <builtins/SpectestModule.h>

std::string getCurrentMemoryUseage() {
#ifdef __linux__
    std::ifstream infile("/proc/self/status");
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.find("VmSize:") == 0) {
            std::string value = line.substr(strlen("VmSize:"));
            value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
            return "Memory useage: " + value;
        }
    }
#endif
    return "Unknown memory useage";
}

using namespace wasm_module;
using namespace wasmint;

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

// returns 0 if the program halts, 1 if it will never halt, and 2 if there is a error with the program itself
// and 3 if it can't be decided
int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "No modules given. Call programm like this: \n$ does_halt module1.wasm" << std::endl;
        return 1;
    }

    const Module* mainModule = nullptr;

    WasmintVM vm;

    vm.loadModule(*RandomModule::create(), true);
    vm.loadModule(*StdioModule::create(), true);
    vm.loadModule(*SpectestModule::create(), true);

    for(int i = 1; i < argc; i++) {
        const std::string& modulePath = argv[i];

        try {
            std::cout << "Loading module " << modulePath << std::endl;
            vm.loadModule(modulePath);
        } catch (const std::exception& e) {
            std::cout << "Got exception while parsing sexpr module "
            << modulePath << ": " << e.what() << " (typeid name " << typeid(e).name() << ")"
            << std::endl;
            return 2;
        }
    }
    

    for (const wasm_module::Module *module : vm.modules()) {
        try {
            module->getFunction("$main");
            if (mainModule != nullptr) {
                std::cout << "Multiple modules with a main function! Aborting..." << std::endl;
                std::cout << "Module 1 was " << module->name() << ", Module 2 was " << mainModule->name() <<
                std::endl;
                return 2;
            }
            mainModule = module;
        } catch (const wasm_module::NoFunctionWithName &ex) {
            // has no main function
        }
    }

    if (mainModule == nullptr) {
        std::cout << "No module loaded with a main function! Aborting..." << std::endl;
        return 2;
    }

    try {
        std::cout << "Start analyzing program" << std::endl;

        vm.startAtFunction(*mainModule->function("$main"), true);
        std::cout << "Analysing... " << getCurrentMemoryUseage() << " counter: " << vm.state().instructionCounter().toString() << std::endl;
        while (!vm.finished()) {
            vm.step();
            if (vm.instructionCounter().multipleOf(5000000)) {
                std::cout << "Analysing... " << getCurrentMemoryUseage() << " counter: " << vm.state().instructionCounter().toString() << std::endl;
                HaltingProblemDetector haltingProblemDetector(vm);
                if (haltingProblemDetector.isLooping(0)) {
                    std::cout << std::endl << "Program will never stop" << std::endl;
                    haltingProblemDetector.printInfo();
                    return 1;
                }
                haltingProblemDetector.printInfo();
                vm.history().addCheckpoint(vm.state());
            }
        }
        std::cout << std::endl << "Finished with instruction counter: " << vm.state().instructionCounter().toString() << std::endl;

        if (vm.gotTrap()) {
            std::cout << "Got trap while executing program: " << vm.trapReason() << std::endl;
            return 0;
        }
        return 0;
    } catch(const wasm_module::NoFunctionWithName& e) {
        if (std::string(e.what()) == "$main") {
            std::cout << std::endl << "None of the given modules has a main function. Exiting..." << std::endl;
        } else {
            std::cout << std::endl << "Exiting because we can't find function with name: " << e.what() << std::endl;
        }
        return 2;
    } catch(const CantMakeHaltingDecision& ex) {
        std::cout << std::endl << "Can't decide wether program halts" << std::endl;
        return 3;
    } catch(const std::exception& ex) {
        std::cout << std::endl << "Got exception while executing: " << ex.what() << " (typeid name " << typeid(ex).name() << ")"
        << std::endl;
        return 2;
    }
}
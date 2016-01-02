
#include <iostream>
#include <fstream>
#include <vector>
#include <binary_parsing/ByteStream.h>
#include <Module.h>
#include <binary_parsing/ModuleParser.h>
#include <interpreter/at/MachineState.h>
#include <sexpr_parsing/ModuleParser.h>
#include <sexpr_parsing/SExprParser.h>
#include <builtins/StdioModule.h>
#include <builtins/SDLModule.h>
#include <interpreter/WasmintVM.h>

using namespace wasm_module;
using namespace wasmint;

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

int main(int argc, char** argv) {
    bool runMain = true;

    if (argc == 1) {
        std::cerr << "No modules given. Call programm like this: \n$ wasmint module1.wasm" << std::endl;
        return 1;
    }

    const Module* mainModule = nullptr;

    WasmintVM vm;

#ifdef WASMINT_HAS_SDL
    vm.loadModule(*SDLModule::create(), true);
#endif
    vm.loadModule(*StdioModule::create(), true);

    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg.find("--") == 0) {
            if (arg == "--no-run") {
                runMain = false;
            } else {
                std::cerr << "Unknown argument " << arg << std::endl;
                return 2;
            }
        }


        const std::string& modulePath = argv[i];

        try {
            vm.loadModule(modulePath);
        } catch (const std::exception& e) {
            std::cerr << "Got exception while parsing sexpr module "
            << modulePath << ": " << e.what() << " (typeid name " << typeid(e).name() << ")"
            << std::endl;
            return 1;
        }
    }


    if (runMain) {

        for (const wasm_module::Module *module : vm.modules()) {
            try {
                module->getFunction("main");
                if (mainModule != nullptr) {
                    std::cerr << "Multiple modules with a main function! Aborting..." << std::endl;
                    std::cerr << "Module 1 was " << module->name() << ", Module 2 was " << mainModule->name() <<
                    std::endl;
                    return 1;
                }
                mainModule = module;
            } catch (const wasm_module::NoFunctionWithName &ex) {
                // has no main function
            }
        }

        if (mainModule == nullptr) {
            std::cerr << "No module loaded with a main function! Aborting..." << std::endl;
            return 1;
        }

        try {
            vm.startAtFunction(*mainModule->function("main"), false);
            vm.stepUntilFinished();
            if (vm.gotTrap()) {
                std::cerr << "Got trap while executing program: " << vm.trapReason() << std::endl;
                return 2;
            }
        } catch(const wasm_module::NoFunctionWithName& e) {
            if (std::string(e.what()) == "main") {
                std::cerr << "None of the given modules has a main function. Exiting..." << std::endl;
            } else {
                std::cerr << "Exiting because we can't find function with name: " << e.what() << std::endl;
            }
            return 3;
        } catch(const std::exception& ex) {
            std::cerr << "Got exception while executing: " << ex.what() << " (typeid name " << typeid(ex).name() << ")"
            << std::endl;
            return 1;
        }

    }
}
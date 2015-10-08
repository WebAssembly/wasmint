
#include <iostream>
#include <fstream>
#include <vector>
#include <binary_parsing/ByteStream.h>
#include <Module.h>
#include <binary_parsing/ModuleParser.h>
#include <interpreter/MachineState.h>
#include <interpreter/Thread.h>

using namespace wasm_module;
using namespace wasmint;

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "No modules given. Call programm like this: \n$ wasmint module1.wasm" << std::endl;
        return 1;
    }

    Module* mainModule = nullptr;

    MachineState environment;

    for(int i = 1; i < argc; i++) {
        std::string modulePath = argv[i];

        std::streampos size;
        std::vector<uint8_t> data;

        std::ifstream file(modulePath.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            size = file.tellg();
            data.resize(size);
            file.seekg(0, std::ios::beg);
            file.read((char *) data.data(), size);
            file.close();
        }
        else std::cerr << "Unable to open module " << modulePath;

        binary::ByteStream stream(std::deque<uint8_t>(data.begin(), data.end()));

        try {
            Module *m = binary::ModuleParser::parse(stream);
            environment.useModule(*m);
            try {
                m->getFunction("main");

                if (mainModule != nullptr) {
                    std::cerr << "Multiple modules with a main function! Aborting..." << std::endl;
                    std::cerr << "Module 1 was " << m->name() << ", Module 2 was " << mainModule->name() << std::endl;
                    return 1;
                }
                mainModule = m;
            } catch (const std::exception& ex) {
                // has no main function
            }

        } catch (std::exception e) {
            std::cerr << "Got exception while parsing module " << modulePath << ": " << e.what() << std::endl;
        }
    }

    if (mainModule == nullptr) {
        std::cerr << "No module loaded with a main function! Aborting..." << std::endl;
        return 1;
    }

    try {
        Thread& thread = environment.createThread().startAtFunction(mainModule->name(), "main");
        thread.stepUntilFinished();
    } catch(wasm_module::NoFunctionWithName e) {
        if (e.what() == "main") {
            std::cerr << "None of the given modules has a main function. Exiting..." << std::endl;
        }
    }
    std::cout << environment.stdout() << std::endl;
}
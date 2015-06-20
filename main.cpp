
#include <iostream>
#include <fstream>
#include <vector>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "No modules given. Call programm like this: \n$ wasmint module1.wasm" << std::endl;
        return 1;
    }

    Environment environment;

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

        ByteStream stream(std::deque<uint8_t>(data.begin(), data.end()));

        try {
            Module *m = ModuleParser::parse(stream);
            environment.useModule(*m);
        } catch (std::exception e) {
            std::cerr << "Got exception while parsing module " << modulePath << ": " << e.what() << std::endl;
        }
    }

    try {
        environment.callFunction("main");
    } catch(NoFunctionWithName e) {
        if (e.what() == "main") {
            std::cerr << "None of the given modules has a main function. Exiting..." << std::endl;
        }
    }
    std::cout << environment.stdout() << std::endl;
}
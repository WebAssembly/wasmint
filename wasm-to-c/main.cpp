
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
#include "ModuleConverter.h"

using namespace wasm_module;
using namespace wasmint;

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "No modules given. Call programm like this: \n$ wasm2c module1.wasm" << std::endl;
        return 1;
    }

    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        const std::string& modulePath = argv[i];

        Module* m;

        bool binary = !ends_with(modulePath, ".wasm");

        if (binary) {
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
                m = binary::ModuleParser::parse(stream);
                m->name(modulePath);
            } catch (const std::exception& e) {
                std::cerr << "Got exception while parsing binary module "
                << modulePath << ": " << e.what() << " (typeid name " << typeid(e).name() << ")"
                << std::endl;
                return 1;
            }

        } else {

            std::ifstream moduleFile(modulePath);
            std::string moduleData((std::istreambuf_iterator<char>(moduleFile)),
                            std::istreambuf_iterator<char>());
            sexpr::CharacterStream stream(moduleData);

            try {
                sexpr::SExpr expr = sexpr::SExprParser(stream).parse(true);
                m = sexpr::ModuleParser::parse(expr[0]);
            } catch (const std::exception& e) {
                std::cerr << "Got exception while parsing sexpr module "
                << modulePath << ": " << e.what() << " (typeid name " << typeid(e).name() << ")"
                << std::endl;
                return 1;
            }
        }
        ModuleConverter converter(m);
        std::cout << converter.cSource() << std::endl;
    }
}
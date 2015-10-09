#ifndef WASMINT_STDIOMODULE_H
#define WASMINT_STDIOMODULE_H


#include <Module.h>
#include <types/Int32.h>
#include <iostream>

class StdioModule {

public:
    static wasm_module::Module* create() {
        using namespace wasm_module;
        Module* module = new Module();
        module->context().name("\"stdio\"");

        module->addFunction("\"print\"", Void::instance(), {Int32::instance()}, [](std::vector<Variable> parameters) {
            std::cout << "print_i32 " << Int32::getValue(parameters[0]) << std::endl;
            return Void::instance();
        });
        return module;
    }
};


#endif //WASMINT_STDIOMODULE_H

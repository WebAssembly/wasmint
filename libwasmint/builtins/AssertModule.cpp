#include "AssertModule.h"

wasm_module::Module *wasmint::AssertModule::create() {
    using namespace wasm_module;
    Module* module = new Module();
    module->context().name("assert");

    module->addFunction("assert_eq_i32", Void::instance(), {Int32::instance(), Int32::instance()}, [](std::vector<Variable> parameters) {
        const wasm_module::Instruction* instruction = currentThread_->getRootInstructionState().parent()->instruction();

        int32_t left = parameters.at(0).int32();
        int32_t right = parameters.at(1).int32();

        uint32_t uleft = parameters.at(0).uint32();
        uint32_t uright = parameters.at(1).uint32();


        if (left != right) {
            std::stringstream stream;
            stream << std::hex << "0x" << uleft << " is not equal " << "0x" << uright;
            std::string hexError(stream.str());

            throw std::domain_error(
                    std::string("Assertion failed (int32):\n") + std::to_string(left) + " is not equal " +
                    std::to_string(right) + "\nhex:" +
                    hexError + "\n" + "unsigned:" + std::to_string(uleft) + " is not equal " +
                    std::to_string(uright) + "\n" +
                    instruction->toSExprString()
            );

        }
        return Void::instance();
    });

    module->addFunction("assert_trap", Void::instance(), {Int32::instance(), Int32::instance()}, [](std::vector<Variable> parameters) {
        // TODO
        return Void::instance();
    });

    return module;
}
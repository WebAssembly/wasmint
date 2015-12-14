#ifndef WASMINT_UNREACHABLEVALIDATOR_H
#define WASMINT_UNREACHABLEVALIDATOR_H


namespace wasm_module {
    class Instruction;

    class UnreachableValidator {
    public:
        static bool willNeverEvaluate(const Instruction *instruction);
    };
}


#endif //WASMINT_UNREACHABLEVALIDATOR_H

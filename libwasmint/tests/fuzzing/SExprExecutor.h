#ifndef WASMINT_SEXPREXECUTOR_H
#define WASMINT_SEXPREXECUTOR_H

#include <interpreter/at/MachineState.h>
#include <sexpr_parsing/CharacterStream.h>
#include <sexpr_parsing/SExpr.h>
#include <sexpr_parsing/SExprParser.h>
#include <sexpr_parsing/ModuleParser.h>
#include <interpreter/at/thread/InterpreterThread.h>
#include <iostream>

namespace wasmint {
    class SExprExecutor {

        MachineState machineState;

    public:
        SExprExecutor() {

        }

        void execute(const std::string& program) {
            using namespace wasm_module;
            using namespace wasmint;
            sexpr::StringCharacterStream stream(program);

            Module* m = nullptr;
            try {
                sexpr::SExpr expr = sexpr::SExprParser(stream).parse(true);
                m = sexpr::ModuleParser::parse(expr[0]);
                InterpreterThread & thread = machineState.createThread().startAtFunction(m->name(), "$main");
                thread.stepUntilFinished();
            } catch (const std::exception& e) {
                // uncommented because it only slows down the normal fuzzing
                //std::cout << "Got exception with mangled name " << typeid(e).name() << " and what(): " << e.what() << std::endl;
            }
            if (m)
                delete m;
        }

    };
}



#endif //WASMINT_SEXPREXECUTOR_H

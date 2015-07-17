

#ifndef WASMINT_FUNCTIONCALL_H
#define WASMINT_FUNCTIONCALL_H


#include <instructions/Instruction.h>
#include <ModuleContext.h>

class FunctionCall : public Instruction {

    FunctionSignature functionSignature;

public:
    FunctionCall(ByteStream& stream, ModuleContext& context) {
        functionSignature = context.functionTable().getFunctionSignature(stream.popULEB128());
    }

    virtual std::string name() {
        return "call_direct";
    }

    virtual std::vector<Type*> childrenTypes() {
        return functionSignature.parameters();
    }

    virtual Type* returnType() {
        return functionSignature.returnType();
    }

    virtual StepResult execute(Thread &thread) {
        InstructionState& state = thread.getInstructionState();
        if (state.state() < children().size()) {
            return StepResult(children().at(0));
        } else if (state.state() == children().size()) {
            std::vector<Variable> parameters;
            for(uint32_t i = 0; i < parameters.size(); i++) {
                parameters[i] = state.results().at(i);
            }
            return StepResult(thread.callFunction(functionSignature.name()));
        } else {
            return state.results().back();
        }
    }
};


#endif //WASMINT_FUNCTIONCALL_H

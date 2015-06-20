//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_INSTRUCTIONSET_H
#define WASMINT_INSTRUCTIONSET_H

#include <string>
#include "Instruction.h"

ExceptionMessage(UnknownInstructionName)

class InstructionSet {

public:
    static Instruction * getInstruction(std::string name, ByteStream& stream, ModuleContext& context, FunctionContext& functionContext);
};


#endif //WASMINT_INSTRUCTIONSET_H

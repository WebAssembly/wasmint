//
// Created by teemperor on 19.06.15.
//

#include "InstructionSet.h"
#include <instructions/I32/I32Add.h>
#include <instructions/Print.h>
#include <instructions/GetLocal.h>
#include <instructions/controlflow/Block.h>
#include <instructions/SetLocal.h>
#include <instructions/controlflow/Break.h>
#include <instructions/controlflow/Continue.h>
#include <instructions/controlflow/DoWhile.h>
#include <instructions/controlflow/If.h>
#include <instructions/controlflow/Return.h>
#include <instructions/controlflow/Forever.h>
#include <instructions/FunctionCall.h>

Instruction *InstructionSet::getInstruction(std::string name, ByteStream& stream, ModuleContext& context, FunctionContext& functionContext) {
    if (name == "int32.add") {
        return new I32Add();
    } else if (name == "print") {
        return new Print();
    } else if (name == "call") {
        return new FunctionCall(stream, context);
    } else if (name == "get_local") {
        return new GetLocal(stream, functionContext);
    } else if (name == "block") {
        return new Block(stream);
    } else if (name == "set_local") {
        return new SetLocal(stream);
    } else if (name == "break") {
        return new Break();
    } else if (name == "continue") {
        return new Continue();
    } else if (name == "do_while") {
        return new DoWhile();
    } else if (name == "forever") {
        return new Forever();
    } else if (name == "if") {
        return new If();
    } else if (name == "return") {
        return new Return();
    } else {
        throw UnknownInstructionName(name);
    }
}
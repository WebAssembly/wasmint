//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_INSTRUCTIONSET_H
#define WASMINT_INSTRUCTIONSET_H

#include <string>
#include <instructions/I32Add.h>
#include <parsing/ByteStream.h>
#include <instructions/Print.h>
#include <instructions/GetLocal.h>
#include <instructions/Block.h>
#include <instructions/SetLocal.h>
#include <instructions/Break.h>
#include <instructions/Continue.h>
#include <instructions/DoWhile.h>
#include <instructions/Forever.h>
#include <instructions/If.h>
#include <instructions/Return.h>
#include <instructions/FunctionCall.h>
#include "Instruction.h"

class UnknownInstructionName : public std::exception {};

class InstructionSet {

public:
    static Instruction* getInstruction(std::string name, ByteStream& stream, ModuleContext& context) {
        if (name == "int32.add") {
            return new I32Add();
        } else if (name == "print") {
            return new Print();
        } else if (name == "call") {
            return new FunctionCall(stream, context);
        } else if (name == "get_local") {
            return new GetLocal(stream);
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
            throw UnknownInstructionName();
        }
    }
};


#endif //WASMINT_INSTRUCTIONSET_H

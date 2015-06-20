

#ifndef WASMINT_MODULECONTEXT_H
#define WASMINT_MODULECONTEXT_H


#include "TypeTable.h"
#include "OpcodeTable.h"
#include "FunctionTable.h"

class ModuleContext {

    OpcodeTable opcodeTable_;
    TypeTable typeTable_;
    FunctionTable functionTable_;

public:
    ModuleContext() {

    }

    ModuleContext(OpcodeTable& opcodeTable, TypeTable& typeTable, FunctionTable& functionTable)
        : opcodeTable_(opcodeTable), typeTable_(typeTable), functionTable_(functionTable) {

    }

    OpcodeTable& opcodeTable() {
        return opcodeTable_;
    }

    TypeTable& typeTable() {
        return typeTable_;
    }

    FunctionTable& functionTable() {
        return functionTable_;
    }

};


#endif //WASMINT_MODULECONTEXT_H

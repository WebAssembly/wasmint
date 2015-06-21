

#ifndef WASMINT_MODULECONTEXT_H
#define WASMINT_MODULECONTEXT_H


#include "TypeTable.h"
#include "OpcodeTable.h"
#include "FunctionTable.h"
#include "GlobalTable.h"

class ModuleContext {

    OpcodeTable opcodeTable_;
    TypeTable typeTable_;
    FunctionTable functionTable_;
    GlobalTable globalTable_;


public:
    ModuleContext() {

    }

    ModuleContext(OpcodeTable& opcodeTable, TypeTable& typeTable, FunctionTable& functionTable, GlobalTable& globalTable)
        : opcodeTable_(opcodeTable), typeTable_(typeTable), functionTable_(functionTable), globalTable_(globalTable) {

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

    GlobalTable& globalTable() {
        return globalTable_;
    }

};


#endif //WASMINT_MODULECONTEXT_H

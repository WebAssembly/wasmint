//
// Created by teemperor on 19.06.15.
//

#ifndef WASMINT_MODULECONTEXT_H
#define WASMINT_MODULECONTEXT_H


#include "TypeTable.h"
#include "OpcodeTable.h"

class ModuleContext {

    OpcodeTable opcodeTable_;
    TypeTable typeTable_;

public:
    ModuleContext() {

    }

    ModuleContext(OpcodeTable& opcodeTable, TypeTable& typeTable)
        : opcodeTable_(opcodeTable), typeTable_(typeTable) {

    }

    OpcodeTable& opcodeTable() {
        return opcodeTable_;
    }

    TypeTable& typeTable() {
        return typeTable_;
    }

};


#endif //WASMINT_MODULECONTEXT_H

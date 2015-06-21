//
// Created by teemperor on 21.06.15.
//

#ifndef WASMINT_GLOBALTABLE_H
#define WASMINT_GLOBALTABLE_H

#include <vector>
#include "Global.h"

#include "ExceptionWithMessage.h"

ExceptionMessage(NoGlobalWithIndex)

class GlobalTable {

    std::vector<Global> globals;
    std::vector<Global> internalGlobals_;

public:
    void addGlobal(Global global, bool internal) {
        globals.push_back(global);
        if (internal)
            internalGlobals_.push_back(global);
    }

    Global& getGlobal(uint32_t globalIndex) {
        if (globalIndex >= globals.size())
            throw NoGlobalWithIndex(std::to_string(globalIndex));

        return globals[globalIndex];
    }

    std::vector<Global> internalGlobals() {
        return internalGlobals_;
    }

};


#endif //WASMINT_GLOBALTABLE_H

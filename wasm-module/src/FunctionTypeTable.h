//
// Created by teemperor on 30.11.15.
//

#ifndef WASMINT_FUNCTIONTYPETABLE_H
#define WASMINT_FUNCTIONTYPETABLE_H

#include <vector>
#include <map>
#include "ExceptionWithMessage.h"
#include "FunctionType.h"
#include "FunctionTable.h"

namespace wasm_module {

    ExceptionMessage(NoFunctionTypeWithAlias)

    class FunctionTypeTable {

        std::vector<FunctionType> types_;
        std::map<std::string, size_t> namesToIndizes_;

    public:
        FunctionTypeTable() {
        }

        void addFunctionType(const FunctionType& type) {
            types_.push_back(type);
            types_.back().index(types_.size() - 1);
        }

        void addFunctionType(const FunctionType& type, std::string alias) {
            types_.push_back(type);
            types_.back().index(types_.size() - 1);
            addAlias(alias, types_.size() - 1);
        }

        void addAlias(const std::string& aliasName, size_t index) {
            namesToIndizes_[aliasName] = index;
        }

        size_t getIndexForAlias(const std::string &alias) const {
            auto iter = namesToIndizes_.find(alias);
            if (iter != namesToIndizes_.end()) {
                return iter->second;
            } else {
                throw NoFunctionTypeWithAlias(alias);
            }
        }

        FunctionType getType(const std::string& alias) const {
            return getType(getIndexForAlias(alias));
        }

        FunctionType getType(size_t index) const {
            return types_.at(index);
        }

        bool operator==(const FunctionTypeTable& other) const {
            return  Utils::compareVector(types_, other.types_) &&
                    Utils::compareMaps(namesToIndizes_, other.namesToIndizes_);
        }
    };
}

#endif //WASMINT_FUNCTIONTYPETABLE_H

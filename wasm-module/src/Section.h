/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INTERPRETER_SECTION_H
#define INTERPRETER_SECTION_H

#include <cstdint>
#include <vector>

#include "ExceptionWithMessage.h"

namespace wasm_module {

    enum class SectionType {
        DATA = 0,
        CODE = 1,
        INVALID = 200
    };

    ExceptionMessage(NoFunctionWithThatName)

    class Function;

    class Section {

        SectionType type_;
        uint32_t offset_;
        std::vector<Function *> functions_;

    public:
        Section(uint32_t offset, SectionType type, std::vector<Function *> functions)
                : offset_(offset), type_(type), functions_(functions) {
        }

        virtual ~Section();

        SectionType type() {
            return type_;
        }

        uint32_t offset() {
            return offset_;
        }

        std::vector<Function *> functions() {
            std::vector<Function *> result;
            for (Function *function : functions_) {
                result.push_back(function);
            }
            return result;
        }

        Function &getFunction(std::string name);
    };
}

#endif //INTERPRETER_SECTION_H

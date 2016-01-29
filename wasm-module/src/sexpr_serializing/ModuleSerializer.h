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


#ifndef WASMINT_MODULESERIALIZER_H
#define WASMINT_MODULESERIALIZER_H

#include <Module.h>

namespace wasm_module { namespace sexpr{
    class ModuleSerializer {

        template<class T>
        static void serializeMemorySegment(T& out, const HeapSegment& segment) {
            out << "\n    (segment ";
            out << segment.offset();
            out << " \"";
            // TODO escape
            for (uint8_t character : segment.data()) {
                out << (char) character;
            }
            out << "\"";
            out << ")\n";
        }

        template<class T>
        static void serializeMemory(T& out, const HeapData& heapData) {
            out << "  (memory ";
            out << heapData.startSize();
            out << " ";
            out << heapData.maxSize();
            for (const HeapSegment& segment : heapData.segments()) {
                serializeMemorySegment(out, segment);
            }
            out << "  )";
        }

        template<class T>
        static void serializeFunctions(T& out, const Function& function, std::size_t functionIndex) {
            out << "\n  (func ";

            if (function.name() != std::to_string(functionIndex)) {
                out << function.name() << "\n    ";
            }

            for (auto& param : function.parameters()) {
                out << "(" << param->name() << ") ";
            }

            out << "    " << function.mainInstruction()->toSExprString();
            out << "\n  )";
        }
    public:
        template<class T>
        static void serialize(T& out, Module* module) {
            out << "module\n";
            serializeMemory(out, module->heapData());
            for (std::size_t i = 0; i < module->functions().size(); i++) {
                serializeFunctions(out, *module->functions().at(i), i);
            }
        }
    };
}}



#endif //WASMINT_MODULESERIALIZER_H

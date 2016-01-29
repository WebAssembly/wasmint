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

#ifndef WASMINT_FLOAT32_H
#define WASMINT_FLOAT32_H


#include <types/Type.h>
#include <Variable.h>

namespace wasm_module {

    class Float32 : public Type {

        bool tryParseNan(const std::string& literal, void *data) const;

    protected:
        Float32() {
        }


    public:
        static const Float32* instance() {
            static Float32 instance;
            return &instance;
        }

        virtual std::string name() const {
            return "f32";
        }


        virtual void parse(const std::string& literal, void *data) const;

        virtual void parse(binary::ByteStream &stream, void *data) const {
            ((uint8_t *) data)[0] = stream.popChar();
            ((uint8_t *) data)[1] = stream.popChar();
            ((uint8_t *) data)[2] = stream.popChar();
            ((uint8_t *) data)[3] = stream.popChar();
        }

        static float getValue(Variable variable) {
            if (variable.type() == *instance()) {
                float result = 0;
                float *data = (float *) variable.value();
                result = *data;
                return result;
            } else {
                throw IncompatibleType();
            }
        }

        static void setValue(Variable& variable, float value) {
            if (variable.type() == *instance()) {
                float *data = (float *) variable.value();
                (*data) = value;
            } else {
                throw IncompatibleType();
            }
        }

        virtual std::size_t size() const {
            static_assert(sizeof(float) == 4u, "wasmint requires that that float is exactly 4 bytes big");
            return 4u;
        }
    };

}

#endif //WASMINT_FLOAT32_H

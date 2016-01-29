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

#ifndef WASMINT_FLOAT64_H
#define WASMINT_FLOAT64_H


#include <types/Type.h>
#include <Variable.h>

namespace wasm_module {

    class Float64 : public Type {

        bool tryParseNan(const std::string& literal, void *data) const;

    protected:
        Float64() {
        }


    public:
        static const Float64* instance() {
            static Float64 instance;
            return &instance;
        }


        virtual std::string name() const {
            return "f64";
        }

        virtual void parse(const std::string& literal, void *data) const;

        virtual void parse(binary::ByteStream &stream, void *data) const {
            ((uint8_t *) data)[0] = stream.popChar();
            ((uint8_t *) data)[1] = stream.popChar();
            ((uint8_t *) data)[2] = stream.popChar();
            ((uint8_t *) data)[3] = stream.popChar();
            ((uint8_t *) data)[4] = stream.popChar();
            ((uint8_t *) data)[5] = stream.popChar();
            ((uint8_t *) data)[6] = stream.popChar();
            ((uint8_t *) data)[7] = stream.popChar();
        }

        static double getValue(Variable variable) {
            if (variable.type() == *instance()) {
                double result = 0;
                double *data = (double *) variable.value();
                result = *data;
                return result;
            } else {
                throw IncompatibleType();
            }
        }

        static void setValue(Variable& variable, double value) {
            if (variable.type() == *instance()) {
                double *data = (double *) variable.value();
                (*data) = value;
            } else {
                throw IncompatibleType();
            }
        }

        virtual std::size_t size() const {
            static_assert(sizeof(double) == 8u, "wasmint requires that that double is exactly 8 bytes big");
            return 8u;
        }
    };

}

#endif //WASMINT_FLOAT64_H

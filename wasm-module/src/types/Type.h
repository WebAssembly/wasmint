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

#ifndef WASMINT_TYPE_H
#define WASMINT_TYPE_H


#include <cstdint>
#include <binary_parsing/ByteStream.h>
#include <typeinfo>
#include <Variable.h>

namespace wasm_module {

    class IncompatibleType : public std::exception {
    };

    class Type;

    class Type {

        template <typename A, typename B, typename U = std::less<const Type*>>
        bool ptrless(A a, B b, U u = U()) const {
            return u(a, b);
        }

    protected:
        Type() {
        }

    public:
        Type(const Type &type); // no copy
        Type& operator=(const Type& other); // no copy

        virtual const Type* localType() const {
            return this;
        }

        virtual std::vector<uint8_t> convertFromMemoryType(std::vector<uint8_t> bytes, Type *memoryType) const {
            return bytes;
        }

        virtual std::string name() const = 0;

        virtual void parse(binary::ByteStream &stream, void *data) const = 0;

        virtual void parse(const std::string& literal, void *data) const = 0;

        virtual std::size_t size() const = 0;

        bool operator==(const Type &other) const {
            return this == &other;
        }

        bool operator!=(const Type &other) const {
            return this != &other;
        }


        bool operator<(const Type& other) const {
            return ptrless(this, &other);
        }

        static bool typeCompatible(const Type* expectedType, const Type* receivedType);
    };

}

#endif //WASMINT_TYPE_H

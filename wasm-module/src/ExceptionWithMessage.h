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

#ifndef WASMINT_EXCEPTIONWITHDATA_H
#define WASMINT_EXCEPTIONWITHDATA_H


#include <exception>
#include <string>
#include <binary_parsing/ByteStream.h>


class ExceptionWithMessage : public std::exception {

    std::string message_;

public:
    ExceptionWithMessage(std::string message) : message_(message) {
    }

    virtual const char *what() const noexcept {
        return message_.c_str();
    }

};

#define ExceptionMessage(Name) class Name : public ExceptionWithMessage { \
    public: Name(std::string message) : ExceptionWithMessage(message) {} \
    };


#endif //WASMINT_EXCEPTIONWITHDATA_H

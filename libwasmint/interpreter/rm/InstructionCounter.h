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


#ifndef WASMINT_INSTRUCTIONCOUNTER_H
#define WASMINT_INSTRUCTIONCOUNTER_H

#include <cstdint>
#include <limits>
#include <string>

namespace wasmint {

    class InstructionCounter {

        uint64_t counter_;

    public:
        InstructionCounter(uint64_t value = 0) : counter_(value) {
        }

        InstructionCounter& operator=(const InstructionCounter& other) {
            this->counter_ = other.counter_;
            return *this;
        }

        InstructionCounter& operator++() {
            counter_++;
            return *this;
        }

        InstructionCounter& operator--() {
            counter_--;
            return *this;
        }

        bool operator==(const InstructionCounter& other) const {
            return counter_ == other.counter_;
        }

        bool operator!=(const InstructionCounter& other) const {
            return counter_ != other.counter_;
        }

        bool operator<(const InstructionCounter& other) const {
            return counter_ < other.counter_;
        }

        bool operator<=(const InstructionCounter& other) const {
            return counter_ <= other.counter_;
        }

        bool operator>(const InstructionCounter& other) const {
            return counter_ > other.counter_;
        }

        bool operator>=(const InstructionCounter& other) const {
            return counter_ >= other.counter_;
        }

        std::string toString() const {
            return std::to_string(counter_);
        }
    };
}



#endif //WASMINT_INSTRUCTIONCOUNTER_H

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

#ifndef WASMINT_VALUESTACK_H
#define WASMINT_VALUESTACK_H

#include <cstdint>
#include <vector>

class ValueStack {

    std::vector<uint64_t> stack_;

public:
    template<typename T>
    void push(T value) {
        uint64_t memory = 0;
        *(reinterpret_cast<T*>(&memory)) = value;
        stack_.push_back(memory);
    }

    template<typename T>
    T peek() {
        uint64_t memory = stack_.back();
        return *(reinterpret_cast<T*>(&memory));
    }

    template<typename T>
    T pop() {
        auto result = peek<T>();
        stack_.pop_back();
        return result;
    }

    bool operator==(const ValueStack& other) const{
        return stack_ == other.stack_;
    }

    bool operator!=(const ValueStack& other) const{
        return stack_ != other.stack_;
    }

    bool empty() const {
        return stack_.empty();
    }
};


#endif //WASMINT_VALUESTACK_H

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


#ifndef WASMINT_SAFEADDITION_H
#define WASMINT_SAFEADDITION_H

namespace wasmint {
    inline bool safeSizeTAddition(std::size_t a, std::size_t b, std::size_t* res) {
        #if defined(__GNUC__) && __GNUC__ > 4
            return __builtin_add_overflow(a, b, res);
        #else
            *res = a + b;
            return *res < a || *res < b;
        #endif
    }


    inline bool safeUInt32Addition(uint32_t a, uint32_t b, uint32_t* res) {
        #if defined(__GNUC__) && __GNUC__ > 4
            return __builtin_add_overflow(a, b, res);
        #else
            *res = a + b;
            return *res < a || *res < b;
        #endif
    }
}

#endif //WASMINT_SAFEADDITION_H

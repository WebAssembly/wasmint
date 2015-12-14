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

#include <cstdint>
#include <string>

#ifndef WASMINT_INJECTEDWASMLIB_H

const std::string libWasmSource = R"(

// limit memory to 1 GB for now
size_t _wasm_maxmemorysize = 1000000000;
size_t _wasm_memorysize;
size_t _wasm_pagesize = 4096;
uint8_t* _wasm_heap;


int _wasm_safe_size_t_add(size_t a, size_t b, size_t* res) {
#ifdef __GNUC__
#  include <features.h>
#  if __GNUC_PREREQ(5,0)
    return __builtin_add_overflow(a, b, res);
#  else
    *res = a + b;
    return *res < a || *res < b;
#  endif
#else
    *res = a + b;
    return *res < a || *res < b;
#endif
}

int _wasm_init(size_t startHeapSize) {
    if (startHeapSize == 0) {
        _wasm_heap = 0;
    } else {
        _wasm_heap = (uint8_t*) malloc(startHeapSize);
        if (_wasm_heap == 0) {
            return 1;
        }
    }

    _wasm_memorysize = startHeapSize;

    return 0;
}

int _wasm_deinit() {
    if (_wasm_heap != 0)
        free(_wasm_heap);

    return 0;
}

void _wasm_grow_heap(size_t diff) {
    if (_wasm_safe_size_t_add(_wasm_memorysize, diff, &_wasm_memorysize)) {
        _wasm_deinit();
        fprintf(stderr, "Failed to grow heap (integer overflow). Aborting...");
        abort();
    }
    if (_wasm_memorysize > _wasm_maxmemorysize) {
        _wasm_deinit();
        fprintf(stderr, "Failed to grow heap (bigger than max memory size). Aborting...");
        abort();
    }
    if (realloc(_wasm_heap, _wasm_memorysize) == 0) {
        _wasm_deinit();
        fprintf(stderr, "Failed to grow heap (realloc failed). Aborting...");
        abort();
    }
}

void* _wasm_get_heap(size_t pos, size_t size) {
    if (_wasm_safe_size_t_add(pos, size, &size)) {
        _wasm_deinit();
        fprintf(stderr, "Failed to get data from heap (integer overflow). Aborting...");
        abort();
    }
    if (size > _wasm_memorysize) {
        _wasm_deinit();
        fprintf(stderr, "Failed to get data from heap (out of range). Aborting...");
        abort();
    }
    return _wasm_heap + pos;
}

)";

#define WASMINT_INJECTEDWASMLIB_H

#endif //WASMINT_INJECTEDWASMLIB_H

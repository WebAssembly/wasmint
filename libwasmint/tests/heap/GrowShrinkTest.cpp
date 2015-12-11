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
#include <interpreter/heap/Heap.h>
#include <cassert>
#include <limits>

using namespace wasmint;

int main() {
    Heap heap1, heap2;
    assert (heap1 == heap2);
    heap1.grow(10);
    assert (heap1.size() == 10);
    assert (heap1 != heap2);
    heap1.shrink(9);
    assert (heap1.size() == 1);
    assert (heap1 != heap2);
    heap1.shrink(1);
    assert (heap1.size() == 0);
    assert (heap1 == heap2);

    try {
        heap1.shrink(1);
        assert (false);
    } catch (const CantChangeHeapSize& ex) {
        // expected exception
    }

    try {
        heap1.shrink(std::numeric_limits<std::size_t>::max());
        assert (false);
    } catch (const CantChangeHeapSize& ex) {
        // expected exception
    }

    heap1.shrink(0);
    assert (heap1 == heap2);
    assert (heap1.size() == 0);

    heap1.grow(0);
    assert (heap1 == heap2);
    assert (heap1.size() == 0);

    heap1.grow(1);
    assert (heap1 != heap2);
    assert (heap1.size() == 1);

    try {
        heap1.grow(std::numeric_limits<std::size_t>::max());
        assert (false);
    } catch (const CantChangeHeapSize& ex) {
        // expected exception
    }
}
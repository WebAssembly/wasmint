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
#include <interpreter/heap/HeapMultiPatchPart.h>
#include <cassert>

using namespace wasmint;

int main() {
    HeapMultiPatchPart part1(0, {1, 1});       // 11
    HeapMultiPatchPart part2(2, {1, 1});       //   11
    HeapMultiPatchPart part3(3, {1, 1});       //    11
    HeapMultiPatchPart part4(1, {1, 1});       //  11
    HeapMultiPatchPart part5(1, {1, 1, 1, 1}); //  1111

    assert( part1.overlaps(part1));
    assert(!part1.overlaps(part2));
    assert(!part1.overlaps(part3));
    assert( part1.overlaps(part4));
    assert( part1.overlaps(part5));

    assert(!part2.overlaps(part1));
    assert( part2.overlaps(part2));
    assert( part2.overlaps(part3));
    assert( part2.overlaps(part4));
    assert( part2.overlaps(part5));

    assert(!part3.overlaps(part1));
    assert( part3.overlaps(part2));
    assert( part3.overlaps(part3));
    assert(!part3.overlaps(part4));
    assert( part3.overlaps(part5));

    assert( part4.overlaps(part1));
    assert( part4.overlaps(part2));
    assert(!part4.overlaps(part3));
    assert( part4.overlaps(part4));
    assert( part4.overlaps(part5));

    assert( part5.overlaps(part1));
    assert( part5.overlaps(part2));
    assert( part5.overlaps(part3));
    assert( part5.overlaps(part4));
    assert( part5.overlaps(part5));
}
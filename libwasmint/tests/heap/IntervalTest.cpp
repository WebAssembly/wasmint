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


#include <cassert>
#include <interpreter/heap/Interval.h>

using namespace wasmint;

int main() {
    assert(Interval::withEnd(0, 3).overlaps(Interval::withEnd(0, 3)));
    assert(!Interval::withEnd(4, 5).overlaps(Interval::withEnd(5, 6)));
    assert(!Interval::withEnd(4, 5).overlaps(Interval::withEnd(6, 7)));
    assert(!Interval::withEnd(4, 5).overlaps(Interval::withEnd(1, 2)));
    assert(!Interval::withEnd(4, 5).overlaps(Interval::withEnd(1, 4)));
}
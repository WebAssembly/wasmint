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
#include <cassert>
#include <interpreter/heap/patch/HeapPatch.h>

using namespace wasmint;

class DummyObserver : public HeapObserver {
public:
    HeapPatch* patch;
    virtual void preChanged(const Heap& heap, const Interval& changedInterval) {
        patch->preHeapChanged(heap, changedInterval);
    }

};

DummyObserver observer;

int main() {
    Heap heap;
    heap.resize(100);
    heap.set<uint64_t>(15, 13424556);
    Heap heapBackup = heap;

    HeapPatch heapPatch1(heap);
    observer.patch = &heapPatch1;

    heap.attachObserver(observer);

    assert(heap == heapBackup);
    heap.set<uint64_t>(3, 13424556);
    assert(heap != heapBackup);
    heap.set<uint64_t>(44, 153453459);
    assert(heap != heapBackup);

    heapPatch1.applyPatch(heap);
    assert(heap == heapBackup);

}
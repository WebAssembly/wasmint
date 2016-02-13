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


#include "HaltingProblemDetector.h"
#include <stdexcept>
#include <algorithm>

bool wasmint::HaltingProblemDetector::isLooping(InstructionCounter startCounter) {
    if (startCounter > vm_.instructionCounter()) {
        throw std::domain_error("startCounter is bigger than the current vm instruction counter");
    }

    bool result = false;
    const VMState backupState = vm_.state();

    totalStates_ = vm_.instructionCounter().toUint64();

    InstructionCounter lastCounter = vm_.instructionCounter();
    std::set<std::size_t> pagesThatNeedChecks;

    while (true) {
        const MachinePatch& lastPatch = vm_.history().getCheckpoint(lastCounter);

        if (lastPatch.influencedByExternalState()) {
            throw CantMakeHaltingDecision("Patch indicates that its related state depend on the external state");
        }

        InstructionCounter nextRollbackCounter = lastPatch.startCounter();

        if (nextRollbackCounter < startCounter)
            nextRollbackCounter = startCounter;

        vm_.simulateTo(nextRollbackCounter);

        for (auto iter = pagesThatNeedChecks.begin(); iter != pagesThatNeedChecks.end(); ) {
            std::size_t pageIndex = *iter;
            if (comparePage(vm_.heap(), backupState.heap(), pageIndex)) {
                iter = pagesThatNeedChecks.erase(iter);
            } else {
                ++iter;
            }
        }

        const std::set<std::size_t>& modifiedPages = lastPatch.heapPatch().modifiedChunks();

        bool modifiesRelevantPages = std::includes(modifiedPages.begin(), modifiedPages.end(),
                                                pagesThatNeedChecks.begin(), pagesThatNeedChecks.end());

        pagesThatNeedChecks.insert(modifiedPages.begin(), modifiedPages.end());

        if (modifiesRelevantPages) {
            InstructionCounter counter = nextRollbackCounter;
            while (counter != lastCounter) {
                if (isIdentical(vm_.state(), backupState, pagesThatNeedChecks)) {
                    ignoredStates_ += counter.toUint64();
                    result = true;
                    break;
                }
                ++counter;
                vm_.simulateTo(counter);
            }
        } else {
            ignoredStates_ += (lastCounter.toUint64() - nextRollbackCounter.toUint64());
        }
        if (!result) {
            lastCounter = nextRollbackCounter;
            if (lastCounter <= startCounter) {
                break;
            } else {
                --lastCounter;
            }
        } else {
            break;
        }
    }

    vm_.state() = backupState;
    assert(vm_.state() == backupState);

    return result;
}

bool wasmint::HaltingProblemDetector::isIdentical(const VMState& a, const VMState& b, std::set<std::size_t>& indexes) {
    // we don't compare the instruction pointer which is on purpose
    // as we only compare for memory/thread equality when checking for reoccurring states

    if (a.heap().size() != b.heap().size())
        return false;
    for (std::size_t pageIndex : indexes) {
        if (!comparePage(a.heap(), b.heap(), pageIndex)) {
            return false;
        }
    }
    if (a.thread() != b.thread())
        return false;
    return true;
}

bool wasmint::HaltingProblemDetector::comparePage(const Heap& a, const Heap& b, std::size_t pageIndex) {
    return a.equalRange(b, HeapPatch::chunkSize * pageIndex, HeapPatch::chunkSize * (pageIndex + 1));
}
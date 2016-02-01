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

#ifndef WASMINT_THREADPATCH_H
#define WASMINT_THREADPATCH_H

#include <cstdint>
#include "FunctionFrame.h"
#include "VMState.h"

namespace wasmint {
    class ThreadPatch {

        std::size_t stackSize;
        std::vector<FunctionFrame> savedFrames_;
        std::string trapReason_;
        bool finished_;
        wasm_module::Variable result_;

        std::size_t smallestSavedFrameIndex() const {
            return stackSize - savedFrames_.size();
        }

    public:
        ThreadPatch() {
        }
        ThreadPatch(const VMThread& thread)
                : stackSize(thread.frames_.size()), trapReason_(thread.trapReason_) {
            if (!thread.frames_.empty()) {
                savedFrames_.push_back(thread.frames_.back());
            }
            finished_ = thread.finished_;
            trapReason_ = thread.trapReason_;
            result_ = thread.result_;
        }

        void backupPreShrink(const VMThread& thread) {
            if (thread.frames_.size() - 1 <= smallestSavedFrameIndex() && smallestSavedFrameIndex() > 0) {
                savedFrames_.push_back(thread.frames_.at(smallestSavedFrameIndex() - 1));
            }
        }

        void applyPatch(VMThread& thread) const {
            thread.frames_.resize(stackSize);
            if (!savedFrames_.empty()) {
                std::size_t j = savedFrames_.size() - 1;
                for (std::size_t i = smallestSavedFrameIndex(); i < stackSize; i++) {
                    thread.frames_.at(i) = savedFrames_.at(j);
                    j--;
                }
            }
            thread.currentFrame_ = &thread.frames_.back();
            thread.finished_ = finished_;
            thread.trapReason_ = trapReason_;
            thread.result_ = result_;
        }
    };
}

#endif //WASMINT_THREADPATCH_H

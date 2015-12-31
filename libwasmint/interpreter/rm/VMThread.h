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


#ifndef WASMINT_VMTHREAD_H
#define WASMINT_VMTHREAD_H

#include <cstdint>
#include <vector>
#include <interpreter/heap/Heap.h>
#include "ByteCode.h"
#include "FunctionFrame.h"
#include "InstructionCounter.h"
#include <stdexcept>

namespace wasmint {


    class WasmintVM;
    class ThreadPatch;

    class VMThread {

        friend class ThreadPatch;

        FunctionFrame* currentFrame_ = nullptr;
        std::vector<FunctionFrame> frames_;
        std::string trapReason_;
        WasmintVM* machine_ = nullptr;

        bool finished_ = false;
        uint32_t stackLimit = 10000;

    public:
        VMThread() {
        }

        VMThread(WasmintVM* machine) : machine_(machine) {
        }

        void finishFrame(uint64_t result) {
            if (frames_.empty())
                throw std::domain_error("Can't call finishFrame(): frame stack is empty!");
            frames_.resize(frames_.size() - 1);
            if (!frames_.empty()) {
                currentFrame_ = &frames_.back();
                currentFrame_->passFunctionResult(result);
            }
            else
                finished_ = true;
        }

        FunctionFrame& currentFrame() {
            return *currentFrame_;
        }

        void pushFrame(const FunctionFrame& frame) {
            frames_.push_back(frame);
            currentFrame_ = &frames_.back();
            if (frames_.size() > stackLimit) {
                trap("maximum amount of function frames reached: " + std::to_string(stackLimit));
            }
        }

        void trap(const std::string& reason) {
            finished_ = true;
            trapReason_ = reason;
        }

        bool finished() {
            return finished_;
        }

        bool gotTrap() const {
            return !trapReason_.empty();
        }

        const std::string& trapReason() const {
            return trapReason_;
        }

        void step(Heap& heap) {
            currentFrame_->step(*this, heap);
        }

        WasmintVM& machine() {
            return *machine_;
        }

        void enterFunction(std::size_t functionId);

        void enterFunction(std::size_t functionId, uint32_t parameterSize, uint16_t parameterRegisterOffset);
    };
}




#endif //WASMINT_BYTECODERUNNER_H

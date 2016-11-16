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


    ExceptionMessage(CantStepEmptyThread)

    class WasmintVM;
    class ThreadPatch;

    class VMThread {

        friend class ThreadPatch;

        FunctionFrame* currentFrame_ = nullptr;
        std::vector<FunctionFrame> frames_;
        std::string trapReason_;
        WasmintVM* machine_ = nullptr;

        bool finished_ = false;
        static const uint32_t stackLimit = 100000;
        wasm_module::Variable result_;


    public:
        VMThread() {
        }

        VMThread(WasmintVM* machine) : machine_(machine) {
        }

        VMThread& operator=(const VMThread& other) {
            frames_ = other.frames_;
            trapReason_ = other.trapReason_;
            machine_ = other.machine_;
            finished_ = other.finished_;
            result_ = other.result_;
            currentFrame_ = &frames_.back();
            return *this;
        }

        VMThread(const VMThread& other) {
            (*this) = other;
        }


        void finishFrame(uint64_t result);

        FunctionFrame& currentFrame() {
            return *currentFrame_;
        }

        void pushFrame(const FunctionFrame& frame) {
            frames_.push_back(frame);
            currentFrame_ = &frames_.back();
            if (frames_.size() > stackLimit) {
                trap("call stack exhausted");
            }
        }

        void trap(const std::string& reason) {
            finished_ = true;
            trapReason_ = reason;
        }

        bool finished() const {
            return finished_;
        }

        bool gotTrap() const {
            return !trapReason_.empty();
        }

        const std::string& trapReason() const {
            return trapReason_;
        }

        void step(Heap& heap) {
            if (!currentFrame_)
                throw CantStepEmptyThread("Thread is empty");
            currentFrame_->step(*this, heap);
        }

        bool stepDebug(Heap& heap) {
            if (!currentFrame_)
                throw CantStepEmptyThread("Thread is empty");
            return currentFrame_->stepDebug(*this, heap);
        }

        WasmintVM& machine() {
            return *machine_;
        }

        const wasm_module::Variable& result() const {
            return result_;
        }

        void enterFunction(std::size_t functionId);
        void enterFunction(std::size_t functionId, const std::vector<wasm_module::Variable>& parameters);

        void enterFunction(std::size_t functionId, uint32_t parameterSize);

        bool operator==(const VMThread& other) const {

            if (frames_.size() != other.frames_.size()) {
                return false;
            }

            for (std::size_t i = 0; i < frames_.size(); i++) {
                if (frames_[i] != other.frames_[i]) {
                    return false;
                }
            }

            return result_ == other.result_ && finished_ == other.finished_
                    && trapReason_ == other.trapReason_;
        }

        bool operator!=(const VMThread& other) const {
            return !(*this == other);
        }
    };
}




#endif //WASMINT_BYTECODERUNNER_H

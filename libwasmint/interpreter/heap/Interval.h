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


#ifndef WASMINT_INTERVAL_H
#define WASMINT_INTERVAL_H

#include <cstdint>
#include <cstddef>

namespace wasmint {
    class Interval {
        std::size_t position_;
        std::size_t end_;

        Interval(std::size_t start, std::size_t end) : position_(start), end_(end) {
        }

    public:
        Interval() {
        }

        static Interval withEnd(std::size_t start, std::size_t end) {
            return Interval(start, end);
        }

        bool adjacent(const Interval& other) const {
            return position_ == other.end_ || other.position_ == this->end_;
        }

        bool overlaps(const Interval& other) const {
            return position_ < other.end_ && other.position_ < end_;
        }

        bool adjacentOrOverlaps(const Interval& other) const {
            return position_ <= other.end_ && other.position_ <= end_;
        }

        std::size_t start() const {
            return position_;
        }

        std::size_t end() const {
            return end_;
        }

        void end(std::size_t end) {
            this->end_ = end;
        }

        std::size_t size() const {
            return end_ - position_;
        }

    };
}




#endif //WASMINT_INTERVAL_H

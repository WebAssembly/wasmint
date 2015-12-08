#ifndef WASMINT_HEAPDATA_H
#define WASMINT_HEAPDATA_H


#include "HeapSegment.h"
#include "ExceptionWithMessage.h"

namespace wasm_module {

    ExceptionMessage(NotFittingHeapSegment)
    ExceptionMessage(UnorderedHeapSegment)

    class HeapData {

        uint32_t startSize_ = 0;
        uint32_t maxSize_ = 0;
        std::vector<HeapSegment> segments_;

    public:
        HeapData(uint32_t startSize, uint32_t maxSize);
        HeapData() {
        }

        void addNextSegment(const HeapSegment& segment) {
            if (segment.end() > startSize_) {
                throw NotFittingHeapSegment(
                        "Segment needs a memory size of at least " +
                                std::to_string(segment.offset() + segment.data().size()));
            }
            if (!segments_.empty()) {
                const HeapSegment& lastSegment = segments_.back();
                if (lastSegment.offset() > segment.offset()) {
                    throw UnorderedHeapSegment("Unordered Heap Segment");
                }
                if (lastSegment.end() > segment.offset()) {
                    throw UnorderedHeapSegment("Overlapping Heap Segment");
                }
            }

            segments_.push_back(segment);
        }

        uint32_t startSize() const {
            return startSize_;
        }

        uint32_t maxSize() const {
            return maxSize_;
        }

        const std::vector<HeapSegment>& segments() const {
            return segments_;
        }
    };
}


#endif //WASMINT_HEAPDATA_H

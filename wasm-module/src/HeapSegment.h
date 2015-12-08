#ifndef WASMINT_HEAPSEGMENT_H
#define WASMINT_HEAPSEGMENT_H


#include <cstdint>
#include <vector>

namespace wasm_module {
    class HeapSegment {

        uint32_t offset_ = 0;
        std::vector<uint8_t> data_;

    public:
        HeapSegment() {
        }
        HeapSegment(uint32_t offset, const std::vector<uint8_t> &data) : offset_(offset), data_(data) {
        }

        const std::vector<uint8_t> &data() const {
            return data_;
        }

        uint32_t offset() const {
            return offset_;
        }

        uint32_t end() const {
            // TODO better handling 64 bit
            return (uint32_t) (offset_ + data_.size());
        }

    };
}

#endif //WASMINT_HEAPSEGMENT_H

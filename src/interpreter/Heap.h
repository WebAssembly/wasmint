

#ifndef WASMINT_HEAP_H
#define WASMINT_HEAP_H

#include <cstdint>
#include <vector>
#include <ExceptionWithMessage.h>

ExceptionMessage(OverFlowInHeapAccess)
ExceptionMessage(OutOfBounds)

class Heap {

    std::vector<uint8_t> data;

    bool highestBitSet(uint32_t a) {
        return (a & (1u << 31)) != 0;
    }

    bool addition_is_safe(uint32_t a, uint32_t b) {
        return !highestBitSet(a) && !highestBitSet(b);
    }

public:
    Heap(uint32_t size) {
        data.resize(size);
    }

    std::vector<uint8_t> getBytes(uint32_t offset, uint32_t size) {
        if (!addition_is_safe(offset, size))
            throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                       + " + size " + std::to_string(size));

        if (offset + size > data.size()) {
            throw OutOfBounds(std::string("Offset ") + std::to_string(offset)
                              + " + size " + std::to_string(size));
        }

        std::vector<uint8_t> result;
        result.resize(size);

        for(uint32_t i = offset; i < offset + size; i++) {
            result[i - offset] = data[i];
        }
        return result;
    }

    void setBytes(uint32_t offset, std::vector<uint8_t> bytes) {
        if (!addition_is_safe(offset, (uint32_t) bytes.size()))
            throw OverFlowInHeapAccess(std::string("Offset ") + std::to_string(offset)
                                       + " + size " + std::to_string(bytes.size()));

        if (offset + bytes.size() > data.size()) {
            throw OutOfBounds(std::string("Offset ") + std::to_string(offset)
                              + " + size " + std::to_string(bytes.size()));
        }

        for(uint32_t i = offset; i < offset + bytes.size(); i++) {
            data[i] = bytes[i - offset];
        }
    }

};


#endif //WASMINT_HEAP_H

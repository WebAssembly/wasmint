
#include <cstdint>
#include <cassert>
#include <interpreter/Heap.h>
#include <limits>

int main() {
    uint32_t size = 1024;
    Heap heap(size);

    std::vector<uint8_t> bytes = {1, 2, 3, 4};
    heap.setBytes(0, bytes);

    {
        std::vector<uint8_t> readBytes = heap.getBytes(0, 4);
        assert(readBytes.size() == 4);
        assert(readBytes[0] == 1);
        assert(readBytes[1] == 2);
        assert(readBytes[2] == 3);
        assert(readBytes[3] == 4);
    }

    {
        std::vector<uint8_t> readBytes = heap.getBytes(0, size);
        assert(readBytes.size() == size);
        assert(readBytes[0] == 1);
        assert(readBytes[1] == 2);
        assert(readBytes[2] == 3);
        assert(readBytes[3] == 4);
    }

    {
    }

    try {
        heap.getBytes(1, size);
        assert(false);
    } catch (OutOfBounds e) {
        // everything is fine
    }

    try {
        heap.getBytes(0, size + 1);
        assert(false);
    } catch (OutOfBounds e) {
        // everything is fine
    }

    /**
     * Overflow tests
     */

    try {
        heap.getBytes(std::numeric_limits<uint32_t>::max()/2, std::numeric_limits<uint32_t>::max()/2);
        assert(false);
    } catch (OutOfBounds e) {
        // should only throw OutOfBonds, but no exception about overflows
    }
    try {
        heap.getBytes(std::numeric_limits<uint32_t>::max()/2 + 1, std::numeric_limits<uint32_t>::max()/2);
        assert(false);
    } catch (OverFlowInHeapAccess e) {
        // uint32 overflow
    }
    try {
        heap.getBytes(std::numeric_limits<uint32_t>::max()/2, std::numeric_limits<uint32_t>::max()/2 + 1);
        assert(false);
    } catch (OverFlowInHeapAccess e) {
        // uint32 overflow
    }
    try {
        heap.getBytes(std::numeric_limits<uint32_t>::max()/2 + 1, std::numeric_limits<uint32_t>::max()/2 + 1);
        assert(false);
    } catch (OverFlowInHeapAccess e) {
        // uint32 overflow
    }
    try {
        heap.getBytes(std::numeric_limits<uint32_t>::max(), 1);
        assert(false);
    } catch (OverFlowInHeapAccess e) {
        // uint32 overflow
    }
    try {
        heap.getBytes(1, std::numeric_limits<uint32_t>::max());
        assert(false);
    } catch (OverFlowInHeapAccess e) {
        // uint32 overflow
    }
}

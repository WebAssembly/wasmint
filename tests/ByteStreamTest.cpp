
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>
#include <cassert>
#include <types/Int32.h>

int main() {
    std::deque<uint8_t> data = {
            0x10, // 16u
            0xE5, 0x8E, 0x26, // 624485u
            0x10, // 16s
            0xE5, 0x8E, 0x26, // 624485s
            0x9b, 0xf1, 0x59 // -624485s
    };

    ByteStream stream(data);

    uint32_t leb = stream.popULEB128();
    assert (leb == 16);

    leb = stream.popULEB128();
    assert (leb == 624485);

    int32_t sleb = Int32::getFromStream(stream);
    assert(sleb == 16);

    sleb = Int32::getFromStream(stream);
    assert(sleb == 624485);

    sleb = Int32::getFromStream(stream);
    assert(sleb == -624485);
}

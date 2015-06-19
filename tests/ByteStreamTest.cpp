
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>

int main() {
    std::deque<uint8_t> data = {
            0x10, // 16
            0xE5, 0x8E, 0x26 // 624485
    };

    ByteStream stream(data);

    uint32_t leb = stream.popLEB128();
    assert (leb == 16);

    leb = stream.popLEB128();
    assert (leb == 624485);
}

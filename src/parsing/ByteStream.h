//
// Created by teemperor on 19.06.15.
//

#ifndef INTERPRETER_BYTESTREAM_H
#define INTERPRETER_BYTESTREAM_H


#include <cstdint>
#include <deque>

class ByteStream {

    std::deque<unsigned char> bytes_;
    uint32_t position_ = 0;

public:
    ByteStream(std::deque<uint8_t> bytes) : bytes_(bytes) {
    }
    ByteStream(const ByteStream& copy); // Don't implement

    virtual uint8_t popChar() {
        position_++;
        uint8_t result = bytes_.front();
        bytes_.pop_front();
        return result;
    }

    virtual uint32_t popUint32() {
        uint32_t result = 0;
        result += popChar();
        result <<= 8;
        result += popChar();
        result <<= 8;
        result += popChar();
        result <<= 8;
        result += popChar();
        return result;
    }

    virtual uint8_t peekChar() {
        return bytes_.front();
    }


    virtual uint64_t peekLEB128() {
        uint64_t result = 0;
        // TODO: Handle more thatn 64bit integers
        for(int i = 0; i < 8; i++) {
            uint8_t byte = popChar();
            result |= byte;
            if (byte <= 128u) {
                break;
            }
            result <<= 8;
        }
        return result;
    }

    virtual uint32_t position() {
        return position_;
    }


};


#endif //INTERPRETER_BYTESTREAM_H

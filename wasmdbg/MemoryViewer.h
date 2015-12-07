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


#ifndef WASMINT_MEMORYVIEWER_H
#define WASMINT_MEMORYVIEWER_H

#include "Widget.h"

class MemoryViewer : public Widget {

    wasmint::Heap* heap_ = nullptr;
    std::size_t position_ = 0;
    std::size_t cursorPosition = 0;
    uint32_t halfPos_ = 0;

    std::size_t bytesPerLine = 0;

    bool isHex(int c) {
        if (c >= '0' && c <= '9') {
            return true;
        }
        if (c >= 'a' && c <= 'f') {
            return true;
        }
        if (c >= 'A' && c <= 'F') {
            return true;
        }
        return false;
    }

    uint8_t hexToInt(int c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }
        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        throw std::domain_error("hexToInt() can't handle: " + std::to_string(c));
    }


    char intToHexDigit(uint8_t value) {
        if (value < 10) {
            return '0' + value;
        } else if (value < 16) {
            return 'A' + (value - 10);
        } else {
            throw std::domain_error("intToHexDigit() can't handle: " + std::to_string(value));
        }
    }

    std::string intToHex(uint8_t value) {
        std::string result;
        result.resize(2);
        result[0] = intToHexDigit(value & 0xF);
        result[1] = intToHexDigit((value >> 4));
        return result;
    }

    void setPosition(int delta) {
        if (delta > 0) {
            std::size_t sDelta = delta;
            // TODO unsafe addition
            if (position_ + delta >= heap_->size()) {
                position_ = (delta + position_) % heap_->size();
            }
        } else {
            std::size_t sDelta = -delta;
            if (sDelta > position_) {
                position_ = heap_->size() - delta;
            }
        }
    }

public:
    MemoryViewer() : Widget(5, 4, 60, 20) {
        setBorder(1);
        setName("Memory");
    }

    void setHeap(wasmint::Heap* heap) {
        heap_ = heap;
    }

    virtual void render() override {
        std::size_t posIter = position_;
        int x = 0;
        int y = 0;

        bytesPerLine = 0;
        while (true) {
            uint8_t byte = heap_->getByte(posIter);

            if (cursorPosition == posIter) {
                if (halfPos_ == 0) {
                    print(x, y, intToHexDigit(byte >> 4), A_REVERSE);
                } else {
                    print(x, y, intToHexDigit(byte >> 4));
                }
                if (halfPos_ == 1) {
                    print(x + 1, y, intToHexDigit(byte & 0xF), A_REVERSE);
                } else {
                    print(x + 1, y, intToHexDigit(byte & 0xF));
                }
            } else {
                print(x, y, intToHexDigit(byte >> 4));
                print(x + 1, y, intToHexDigit(byte & 0xF));
            }

            posIter++;
            x += 3;

            if (y == 0)
                bytesPerLine++;

            if (posIter >= heap_->size())
                break;

            if (x >= getWidth() - 1) {
                x = 0;
                y++;
            }
            if (y >= getHeight()) {
                break;
            }
        }
        refresh();
    }

    virtual bool handleCharacter(int c) override {
        if (isHex(c)) {
            uint8_t data = hexToInt(c);

            if (halfPos_ == 0)
                data <<= 4;

            uint8_t valueInMemory = heap_->getByte(cursorPosition);
            if (halfPos_ == 0) {
                valueInMemory &= 0x0F;
            } else {
                valueInMemory &= 0xF0;
            }
            valueInMemory |= data;

            heap_->setByte(cursorPosition, valueInMemory);

            if (halfPos_ == 0) {
                halfPos_ = 1;
            } else {
                cursorPosition++;
                halfPos_ = 0;
            }
            return true;
        }

        if (c == KEY_LEFT) {
            if (halfPos_ == 1) {
                halfPos_ = 0;
            } else {
                cursorPosition--;
                halfPos_ = 1;
            }
            return true;
        }
        if (c == KEY_RIGHT) {
            if (halfPos_ == 0) {
                halfPos_ = 1;
            } else {
                cursorPosition++;
                halfPos_ = 0;
            }
            return true;
        }
        if (c == KEY_UP) {
            cursorPosition -= bytesPerLine;
            return true;
        }
        if (c == KEY_DOWN) {
            cursorPosition += bytesPerLine;
            return true;
        }
        return false;
    }

};


#endif //WASMINT_MEMORYVIEWER_H

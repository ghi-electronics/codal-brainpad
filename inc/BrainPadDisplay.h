#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "MbedI2C.h"

#include <cstdint>

namespace codal {
    class BrainPadDisplay {
        static const int deviceAddress = 0x78;
        static const size_t vramSize = (128 * 64 / 8) + 1;

        codal::_mbed::I2C i2c;
        uint8_t data[2];
        uint8_t vram[vramSize];

        void drawNativePixel(int x, int y, bool set);
        void writeCommand(int cmd);
        void flush();

    public:
        BrainPadDisplay(Pin& sda, Pin& scl);

        void writeScreenBuffer(uint8_t* buffer);
    };
}

#endif

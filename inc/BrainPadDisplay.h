#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "I2C.h"

#include <cstdint>

namespace codal {
    class BrainPadDisplay {
        static const int deviceAddress = 0x78;
        static const size_t vramSize = (128 * 64 / 8) + 1;

        codal::I2C& i2c;
        uint8_t data[2];
        uint8_t vram[vramSize];

        void drawNativePixel(int x, int y, bool set);
        void writeCommand(int cmd);
        void flush();

    public:
        BrainPadDisplay(codal::I2C& _i2c);

        void flush();
        void drawPoint(int x, int y, bool set);
        void writeScreenBuffer(uint8_t* buffer);
    };
}

#endif

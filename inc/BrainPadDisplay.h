#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "I2C.h"
#include <cstdint>

#define deviceAddress 0x78

namespace codal {
    class BrainPadDisplay {
        static const int vramSize = (128 * 64 / 8) + 1;

        codal::I2C& i2c;
        uint8_t data[2];
        uint8_t vram[vramSize];
        uint16_t address;

        void writeCommand(int cmd);
        void flush();

    public:
        BrainPadDisplay(codal::I2C& _i2c, uint16_t address = deviceAddress);

        void writeScreenBuffer(uint8_t* buffer);
    };
}

#endif

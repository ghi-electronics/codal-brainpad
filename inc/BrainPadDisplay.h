#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "MbedI2C.h"

#include <cstdint>

namespace codal {
   class BrainPadDisplay {
        static const int deviceAddress;

        codal::_mbed::I2C i2c;
        uint8_t data[2];
		uint8_t vram[(128 * 64 / 8) + 1];

		void drawNativePixel(int x, int y, bool set );
        void writeCommand(int cmd);

	public:
        BrainPadDisplay(Pin& sda, Pin& scl);

        void writeScreenBuffer(uint8_t* buffer);
   };
}

#endif

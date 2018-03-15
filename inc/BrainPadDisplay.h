#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "MbedI2C.h"

#include <cstdint>

namespace codal {
   class BrainPadDisplay {  
        static const int deviceAddress;
		
        codal::_mbed::I2C i2c;
        uint8_t data[2];

    public:                
        BrainPadDisplay(Pin& sda, Pin& scl);
    
        void initScreen();
        void writeScreenBuffer(uint8_t* buffer);
        void writeCommand(int cmd);
   };
}

#endif

#ifndef DISPLAY_H
#define DISPLAY_H

#include "Image.h"
#include "MbedI2C.h"

namespace codal {
   class Ssd1306 {
   
     public:
     codal::_mbed::I2C i2c;
     Display(Pin& sda, Pin& scl);
    
    // Initialize the display
    void InitScreen();
	// Write Buffer
	void WriteScreenBuffer(uint8_t * buffer);
	
   };
}
#endif
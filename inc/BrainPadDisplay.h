#ifndef BPDISPLAY_H
#define BPDISPLAY_H
#include "Image.h"
#include "MbedI2C.h"

namespace codal {

   class BrainPadDisplay {  
  
   uint8_t Data[2];
   int DeviceAddress = 0x78;
   typedef unsigned char byte;

     public:
     codal::_mbed::I2C i2c;
     BrainPadDisplay(Pin& sda, Pin& scl);
    
    // Initialize the display
    void InitScreen();

	void WriteScreenBuffer(uint8_t * buffer);

	void WriteCommand(int cmd);

   };
}
#endif
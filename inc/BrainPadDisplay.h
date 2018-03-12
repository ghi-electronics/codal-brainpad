#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "Image.h"
#include "MbedI2C.h"
#include <cstdint>

namespace codal {
   class BrainPadDisplay {  
		private:
		int DeviceAddress = 0x78;
		codal::_mbed::I2C i2c;

		public:
		uint8_t data[2];
				
		BrainPadDisplay(Pin& sda, Pin& scl);
    
		void InitScreen();
		void WriteScreenBuffer(uint8_t* buffer);
		void WriteCommand(int cmd);
   };
}

#endif
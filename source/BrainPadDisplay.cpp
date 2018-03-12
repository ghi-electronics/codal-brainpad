#include "BrainPadDisplay.h"

using namespace codal;

//static BrainPadDisplay *device_instance = NULL;

BrainPadDisplay::BrainPadDisplay(Pin& sda, Pin& scl): i2c(sda, scl)
{
  
  //device_instance = this;
  
}

void BrainPadDisplay::WriteCommand(int cmd) {

            
            Data[1] = (byte)cmd;
            i2c.write(DeviceAddress, Data, 2, 0);
}


 void BrainPadDisplay::InitScreen()
{
                    WriteCommand(0xae);// SSD1306_DISPLAYOFF);                    // 0xAE
                    WriteCommand(0xd5);// SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
                    WriteCommand(0x80);                                  // the suggested ratio 0x80

                    WriteCommand(0xa8);// SSD1306_SETMULTIPLEX);                  // 0xA8
                    WriteCommand(64 - 1);

                    WriteCommand(0xd3);// SSD1306_SETDISPLAYOFFSET);              // 0xD3
                    WriteCommand(0x0);                                   // no offset
                    WriteCommand(0x40);// SSD1306_SETSTARTLINE | 0x0);            // line #0
                    WriteCommand(0x8d);// SSD1306_CHARGEPUMP);                    // 0x8D

                    WriteCommand(0x14);
                    

                    WriteCommand(0x20);// SSD1306_MEMORYMODE);                    // 0x20
                    WriteCommand(0x00);                                  // 0x0 act like ks0108
                    WriteCommand(0xa1);// SSD1306_SEGREMAP | 0x1);
                    WriteCommand(0xc8);// SSD1306_COMSCANDEC);


                    WriteCommand(0xda);// SSD1306_SETCOMPINS);                    // 0xDA
                    WriteCommand(0x12);
                    WriteCommand(0x81);// SSD1306_SETCONTRAST);                   // 0x81

                    //if (false)//vccstate == SSD1306_EXTERNALVCC)
                    //{ WriteCommand(0x9F); }
                    //else {
                    WriteCommand(0xCF);
                    //}

                    WriteCommand(0xd9);// SSD1306_SETPRECHARGE);                  // 0xd9

                    WriteCommand(0xF1);
                 
                    WriteCommand(0xd8);// SSD1306_SETVCOMDETECT);                 // 0xDB
                    WriteCommand(0x40);
                    WriteCommand(0xa4);//SSD1306_DISPLAYALLON_RESUME);           // 0xA4
                    WriteCommand(0xa6);// SSD1306_NORMALDISPLAY);                 // 0xA6

                    WriteCommand(0x2e);// SSD1306_DEACTIVATE_SCROLL);

                    WriteCommand(0xaf);// SSD1306_DISPLAYON);//--turn on oled panel


                    WriteCommand(0x21);// SSD1306_COLUMNADDR);
                    WriteCommand(0);   // Column start address (0 = reset)
                    WriteCommand(128 - 1); // Column end address (127 = reset)
                    WriteCommand(0x22);// SSD1306_PAGEADDR);
                    WriteCommand(0); // Page start address (0 = reset)
                    WriteCommand(7); // Page end address

   }

 

 void BrainPadDisplay::WriteScreenBuffer(uint8_t * buffer)
 {
	 for (int y = 0; y<8; y++) {
		 Data[0] = 0x00;
		 Data[1] = 0xB0 + y;
		 i2c.write(DeviceAddress, Data, 2, 0);     //Set GDDRAM page.

		 Data[0] = 0x40;
		 i2c.write(DeviceAddress, Data, 1, 1);     //Tell controller next bytes are GDDRAM data.

		 i2c.write(DeviceAddress, buffer + 128 * y, 128, 0);

	 }
 }
 
#include "Display.h"

using namespace codal;

static Display *device_instance = NULL;

Display::Display(Pin& sda, Pin& scl): i2c(sda, scl) {

  device_instance = this;
  
}

void Display::Display(int cmd) {

	Data[1] = (byte)cmd;
	i2c.write(DeviceAddress, Data, 2, 0);

}

 void Display::InitScreen() {

	Display_command(0xae);// SSD1306_DISPLAYOFF);                    // 0xAE
	Display_command(0xd5);// SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	Display_command(0x80);                                  // the suggested ratio 0x80
	Display_command(0xa8);// SSD1306_SETMULTIPLEX);                  // 0xA8
	Display_command(64 - 1);
	Display_command(0xd3);// SSD1306_SETDISPLAYOFFSET);              // 0xD3
	Display_command(0x0);                                   // no offset
    Display_command(0x40);// SSD1306_SETSTARTLINE | 0x0);            // line #0
    Display_command(0x8d);// SSD1306_CHARGEPUMP);                    // 0x8D
    Display_command(0x14);                  
    Display_command(0x20);// SSD1306_MEMORYMODE);                    // 0x20
    Display_command(0x00);                                  // 0x0 act like ks0108
    Display_command(0xa1);// SSD1306_SEGREMAP | 0x1);
    Display_command(0xc8);// SSD1306_COMSCANDEC);
    Display_command(0xda);// SSD1306_SETCOMPINS);                    // 0xDA
    Display_command(0x12);
    Display_command(0x81);// SSD1306_SETCONTRAST);                   // 0x81
    Display_command(0xCF);
    Display_command(0xd9);// SSD1306_SETPRECHARGE);                  // 0xd9
    Display_command(0xF1);              
    Display_command(0xd8);// SSD1306_SETVCOMDETECT);                 // 0xDB
    Display_command(0x40);
    Display_command(0xa4);//SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    Display_command(0xa6);// SSD1306_NORMALDISPLAY);                 // 0xA6
    Display_command(0x2e);// SSD1306_DEACTIVATE_SCROLL);
    Display_command(0xaf);// SSD1306_DISPLAYON);//--turn on oled panel
    Display_command(0x21);// SSD1306_COLUMNADDR);
    Display_command(0);   // Column start address (0 = reset)
    Display_command(128 - 1); // Column end address (127 = reset)
    Display_command(0x22);// SSD1306_PAGEADDR);
    Display_command(0); // Page start address (0 = reset)
    Display_command(7); // Page end address
}

 void Display::WriteScreenBuffer(uint8_t * buffer) {

	 for (int y = 0; y<8; y++) {
		 Data[0] = 0x00;
		 Data[1] = 0xB0 + y;
		 i2c.write(DeviceAddress, Data, 2, 0);     //Set GDDRAM page.
		 Data[0] = 0x40;
		 i2c.write(DeviceAddress, Data, 1, 1);     //Tell controller next bytes are GDDRAM data.
		 i2c.write(DeviceAddress, buffer + 128 * y, 128, 0);
	 }
 }
 
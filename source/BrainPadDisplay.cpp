#include "BrainPadDisplay.h"

using namespace codal;

const int BrainPadDisplay::deviceAddress = 0x78;

BrainPadDisplay::BrainPadDisplay(Pin& sda, Pin& scl): i2c(sda, scl) {

}

void BrainPadDisplay::writeCommand(int cmd) {
	data[1] = static_cast<uint8_t>(cmd);

	i2c.write(BrainPadDisplay::deviceAddress, data, 2, 0);
}

void BrainPadDisplay::initScreen() {
	writeCommand(0xae);//--turn off oled panel
	writeCommand(0x00);//---set low column address
	writeCommand(0x10);//---set high column address
	writeCommand(0x40);//--set start line address
	writeCommand(0x81);//--set contrast control register
	writeCommand(0xcf);
	writeCommand(0xa1);//--set segment re-map 95 to 0
	writeCommand(0xa6);//--set normal display
	writeCommand(0xa8);//--set multiplex ratio(1 to 64)
	writeCommand(0x3f);//--1/64 duty
	writeCommand(0xd3);//-set display offset
	writeCommand(0x00);//-not offset
	writeCommand(0xd5);//--set display clock divide ratio/oscillator frequency
	writeCommand(0x80);//--set divide ratio
	writeCommand(0xd9);//--set pre-charge period
	writeCommand(0xf1);
	writeCommand(0xda);//--set com pins hardware configuration
	writeCommand(0x12);
	writeCommand(0xdb);//--set vcomh
	writeCommand(0x40);//--set startline 0x0
	writeCommand(0x8d);//--set Charge Pump enable/disable
	writeCommand(0x14);//--set(0x10) disable
	writeCommand(0xaf);//--turn on oled panel 
}

void BrainPadDisplay::writeScreenBuffer(uint8_t* buffer) {
    for (int y = 0; y < 8; y++) {
         data[0] = 0x00;
         data[1] = 0xB0 + y;
         i2c.write(BrainPadDisplay::deviceAddress, data, 2, 0);

         data[0] = 0x40;
         i2c.write(BrainPadDisplay::deviceAddress, data, 1, 1);     
         i2c.write(BrainPadDisplay::deviceAddress, buffer + 128 * y, 128, 0);
     }
}

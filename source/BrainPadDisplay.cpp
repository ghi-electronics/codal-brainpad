#include "BrainPadDisplay.h"

using namespace codal;

//static BrainPadDisplay *device_instance = NULL;

BrainPadDisplay::BrainPadDisplay(Pin& sda, Pin& scl): i2c(sda, scl) {
  
	//device_instance = this;

}

void BrainPadDisplay::WriteCommand(int cmd) {
	data[1] = static_cast<byte>cmd;
	i2c.write(DeviceAddress, Data, 2, 0);
}

void BrainPadDisplay::InitScreen() {
	WriteCommand(0xae);//--turn off oled panel
	WriteCommand(0x00);//---set low column address
	WriteCommand(0x10);//---set high column address
	WriteCommand(0x40);//--set start line address
	WriteCommand(0x81);//--set contrast control register
	WriteCommand(0xcf);
	WriteCommand(0xa1);//--set segment re-map 95 to 0
	WriteCommand(0xa6);//--set normal display
	WriteCommand(0xa8);//--set multiplex ratio(1 to 64)
	WriteCommand(0x3f);//--1/64 duty
	WriteCommand(0xd3);//-set display offset
	WriteCommand(0x00);//-not offset
	WriteCommand(0xd5);//--set display clock divide ratio/oscillator frequency
	WriteCommand(0x80);//--set divide ratio
	WriteCommand(0xd9);//--set pre-charge period
	WriteCommand(0xf1);
	WriteCommand(0xda);//--set com pins hardware configuration
	WriteCommand(0x12);
	WriteCommand(0xdb);//--set vcomh
	WriteCommand(0x40);//--set startline 0x0
	WriteCommand(0x8d);//--set Charge Pump enable/disable
	WriteCommand(0x14);//--set(0x10) disable
	WriteCommand(0xaf);//--turn on oled panel 
}
 
 void BrainPadDisplay::WriteScreenBuffer(uint8_t * buffer) {

	 for (int y = 0; y < 8; y++) {
		 data[0] = 0x00;
		 data[1] = 0xB0 + y;
		 i2c.write(DeviceAddress, Data, 2, 0);     //Set GDDRAM page.
		 data[0] = 0x40;
		 i2c.write(DeviceAddress, Data, 1, 1);     //Tell controller next bytes are GDDRAM data.
		 i2c.write(DeviceAddress, buffer + 128 * y, 128, 0);
	 }
 }
 
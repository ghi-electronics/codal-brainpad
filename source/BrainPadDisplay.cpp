#include "BrainPadDisplay.h"

using namespace codal;

const int BrainPadDisplay::deviceAddress;
const size_t BrainPadDisplay::vramSize;

BrainPadDisplay::BrainPadDisplay(codal::I2C& i2c;) : i2c(_i2c) {
    writeCommand(0xae);// turn off oled panel
    writeCommand(0x00);// set low column address
    writeCommand(0x10);// set high column address
    writeCommand(0x40);// set start line address
    writeCommand(0x81);// set contrast control register
    writeCommand(0xcf);
    writeCommand(0xa1);// set segment re-map 95 to 0
    writeCommand(0xa6);// set normal display
    writeCommand(0xa8);// set multiplex ratio(1 to 64)
    writeCommand(0x3f);// 1/64 duty
    writeCommand(0xd3);// set display offset
    writeCommand(0x00);// not offset
    writeCommand(0xd5);// set display clock divide ratio/oscillator frequency
    writeCommand(0x80);// set divide ratio
    writeCommand(0xd9);// set pre-charge period
    writeCommand(0xf1);
    writeCommand(0xda);// set com pins hardware configuration
    writeCommand(0x12);
    writeCommand(0xdb);//--set vcomh
    writeCommand(0x40);//--set startline 0x0
    writeCommand(0x8d);//--set Charge Pump enable/disable
    writeCommand(0x14);//--set(0x10) disable
    writeCommand(0xaf);//--turn on oled panel
    writeCommand(0xc8);// mirror the screen

    // Mapping
    writeCommand(0x20);
    writeCommand(0x00);
    writeCommand(0x21);
    writeCommand(0);
    writeCommand(128 - 1);
    writeCommand(0x22);
    writeCommand(0);
    writeCommand(7);

    memset(vram, 0, BrainPadDisplay::vramSize);

    vram[0] = 0x40;

    flush();
}

void BrainPadDisplay::writeCommand(int cmd) {
    data[0] = 0;
    data[1] = static_cast<uint8_t>(cmd);

    i2c.write(BrainPadDisplay::deviceAddress, data, 2, 0);
}

void BrainPadDisplay::drawNativePixel(int x, int y, bool set) {
    int index = (x + (y / 8) * 128) + 1;

    if (set)
        vram[index] |= static_cast<uint8_t>(1 << (y % 8));
    else
        vram[index] &= static_cast<uint8_t>(~(1 << (y % 8)));
}

void BrainPadDisplay::flush() {
    i2c.write(BrainPadDisplay::deviceAddress, vram, BrainPadDisplay::vramSize, 0);
}

void BrainPadDisplay::writeScreenBuffer(uint8_t* buffer) {
    for (int x = 0; x < 128; x++) {
        for (int y = 0; y < 64; y++) {
            int offset = y * 16 + x / 8;
            int mask = 0x80 >> (x & 7);

            drawNativePixel(x, y, (buffer[offset] & mask) > 0);
        }
    }

    flush();
}

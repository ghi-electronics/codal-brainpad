#include "BrainPad.h"
#include "BrainPadDisplay.h"

BrainPad brain;

// ------------Draw pixels the PXT way---------------- //

uint8_t PXTvram[1024];

void DrawPointInPXTFormat(int x, int y, bool set = true) {
    if (x >= 0 && x < 128 && y >= 0 && y < 64) {
        //original code: https://github.com/Microsoft/pxt-common-packages/blob/master/libs/screen/image.cpp#L55
        int offset = y * 16;
        offset += x / 8;
        //original code: https://github.com/Microsoft/pxt-common-packages/blob/master/libs/screen/image.cpp#L142
        int mask = 0x80 >> (x & 7);

        if (set)
            PXTvram[offset] |= mask;
        else
            PXTvram[offset] &= ~mask;
    }
}

void TestDisplay() {
    DrawCircleInPXTFormat(64, 32, 30);
    DrawCircleInPXTFormat(64, 32, 20);
    DrawCircleInPXTFormat(64, 32, 10);
    DrawPointInPXTFormat(64, 32);
    brain.lcd.writeScreenBuffer(PXTvram);
}
// ------------------------------------------ //

void OnClick(Event e) {
    brain.serial.printf("CLICK\r\n");
}

int main() {
    brain.init();
    brain.serial.printf(" *** BRAINPAD BLINKY TEST ***\r\n");
    brain.messageBus.listen(DEVICE_ID_BUTTON_A, DEVICE_BUTTON_EVT_CLICK, OnClick);

    TestDisplay();

    while (true) {
        brain.io.led.setDigitalValue(1);
        brain.sleep(100);
        brain.io.led.setDigitalValue(0);
        brain.sleep(100);
    }
}

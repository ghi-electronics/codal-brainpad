#include "BrainPad.h"
#include "BrainPadDisplay.h"

BrainPad brain;

uint8_t buffer[1024];

void SetPixel(int x, int y, bool set) {
    if (x >= 0 && x < 128 && y >= 0 && y < 64) {
        int offset = (x + (y / 8) * 128) + 1;
        int bit = 1 << (y % 8);
        
        if (set) {
            buffer[offset] |= bit;
        }
        else {
            buffer[offset] &= ~bit;
        }
    }
}

void DrawCircle(int x0, int y0, int radius) {
    int x = 0, y = radius;
    int dp = 1 - radius;
    
    do {
        if (dp < 0) {
            dp = dp + 2 * (++x) + 3;
        }
        else {
            dp = dp + 2 * (++x) - 2 * (--y) + 5;
        }
            
        SetPixel(x0 + x, y0 + y, true);
        SetPixel(x0 - x, y0 + y, true);
        SetPixel(x0 + x, y0 - y, true);
        SetPixel(x0 - x, y0 - y, true);
        SetPixel(x0 + y, y0 + x, true);
        SetPixel(x0 - y, y0 + x, true);
        SetPixel(x0 + y, y0 - x, true);
        SetPixel(x0 - y, y0 - x, true);
    } while (x < y);

    SetPixel(x0 + radius, y0, true);
    SetPixel(x0, y0 + radius, true);
    SetPixel(x0 - radius, y0, true);
    SetPixel(x0, y0 - radius, true);
}

void TestDisplay() {
    DrawCircle(20, 20, 10);
    
    brain.lcd.InitScreen();
    brain.lcd.WriteScreenBuffer(buffer);
}

void OnClick(Event e) {
    brain.serial.printf("CLICK\r\n");
}

int main() {
    brain.init();
    brain.serial.printf(" *** BRAINPAD BLINKY TEST ***\r\n");
    brain.messageBus.listen(DEVICE_ID_BUTTON_A, DEVICE_BUTTON_EVT_CLICK, OnClick);
    
    TestDisplay();

    while(1) {
        brain.io.led.setDigitalValue(1);
        brain.sleep(200);
        brain.io.led.setDigitalValue(0);
        brain.sleep(200);
    }
}

#include "BrainPad.h"
#include "BrainPadDisplay.h"

BrainPad brain;

uint8_t ScreenBuffer[1024];

int main() {
    brain.init();
    brain.serial.printf(" *** BRAINPAD BLINKY TEST ***\r\n");
    brain.messageBus.listen(DEVICE_ID_BUTTON_A, DEVICE_BUTTON_EVT_CLICK, onClick);
	TestDisplay();

    while(1) {
        brain.io.led.setDigitalValue(1);
        brain.sleep(200);
        brain.io.led.setDigitalValue(0);
        brain.sleep(200);
    }
}

void Pixel(int x, int y, bool set) {
	if (x >= 0 && x < 128 && y >= 0 && y < 64) {
		if (set) {
			ScreenBuffer[(x + (y / 8) * 128) + 1] |= (1 << (y % 8));
		}
		else {
			ScreenBuffer[(x + (y / 8) * 128) + 1] &= (~(1 << (y % 8)));
		}
	}
}

void SetPixel(int x, int y) {
	Pixel(x, y, true);
}

void DrawCircle(int x0, int y0, int radius) {
	int x = 0, y = radius;
	int dp = 1 - radius;
	do {
		if (dp < 0)
			dp = dp + 2 * (++x) + 3;
		else
			dp = dp + 2 * (++x) - 2 * (--y) + 5;

		SetPixel(x0 + x, y0 + y); 
		SetPixel(x0 - x, y0 + y);
		SetPixel(x0 + x, y0 - y);
		SetPixel(x0 - x, y0 - y);
		SetPixel(x0 + y, y0 + x);
		SetPixel(x0 - y, y0 + x);
		SetPixel(x0 + y, y0 - x);
		SetPixel(x0 - y, y0 - x);
	} while (x < y);

	SetPixel(x0 + radius, y0);
	SetPixel(x0, y0 + radius);
	SetPixel(x0 - radius, y0);
	SetPixel(x0, y0 - radius);
}

void TestDisplay() {
	DrawCircle(20, 20, 10);
	brain.lcd.InitScreen();
	brain.lcd.WriteScreenBuffer(ScreenBuffer);
}

void Pixel(int x, int y, bool set) {
	if (x >= 0 && x < 128 && y >= 0 && y < 64) {
		if (set) {
			ScreenBuffer[(x + (y / 8) * 128) + 1] |= (1 << (y % 8));
		}
		else {
			ScreenBuffer[(x + (y / 8) * 128) + 1] &= (~(1 << (y % 8)));
		}
	}
}
void SetPixel(int x, int y) {
	Pixel(x, y, true);
}

void DrawCircle(int x0, int y0, int radius) {
	int x = 0, y = radius;
	int dp = 1 - radius;
	do {
		if (dp < 0)
			dp = dp + 2 * (++x) + 3;
		else
			dp = dp + 2 * (++x) - 2 * (--y) + 5;

		SetPixel(x0 + x, y0 + y);
		SetPixel(x0 - x, y0 + y);
		SetPixel(x0 + x, y0 - y);
		SetPixel(x0 - x, y0 - y);
		SetPixel(x0 + y, y0 + x);
		SetPixel(x0 - y, y0 + x);
		SetPixel(x0 + y, y0 - x);
		SetPixel(x0 - y, y0 - x);
	} while (x < y);

	SetPixel(x0 + radius, y0);
	SetPixel(x0, y0 + radius);
	SetPixel(x0 - radius, y0);
	SetPixel(x0, y0 - radius);
}

void TestDisplay() {
	DrawCircle(20, 20, 10);
	brain.lcd.InitScreen();
	brain.lcd.WriteScreenBuffer(ScreenBuffer);
}

void onClick(Event) {
    brain.serial.printf("CLICK\r\n");
}




#include "GraphicsHelper.h"


using namespace codal;

GraphicsHelper::GraphicsHelper() {

}

void GraphicsHelper::DrawPoint(int x, int y, bool set) {
    if (x >= 0 && x < 128 && y >= 0 && y < 64) {
        //original code: https://github.com/Microsoft/pxt-common-packages/blob/master/libs/screen/image.cpp#L55
        int offset = y * 16;
        offset += x / 8;
        //original code: https://github.com/Microsoft/pxt-common-packages/blob/master/libs/screen/image.cpp#L142
        int mask = 0x80 >> (x & 7);

        if (set)
            vram[offset] |= mask;
        else
            vram[offset] &= ~mask;
    }
}

void GraphicsHelper::DrawText(int x, int y, char letter) {
    int index = 5 * (letter - 32);

    for (int h = 0; h < 5; h++) {
                
        for (int v = 0; v < 8; v++) {
			int show = (font[index + h] & (1 << v)) != 0;
            DrawPoint(x + h  , y + v , show);       
        }
    }
	// clear the space between characters
	for(int i=0;i<8;i++)
		DrawPoint(x + 5, y+i, 0);
}
void GraphicsHelper::DrawString(int x, int y, std::string text) {
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] >= 32) {
            DrawText(x, y, text[i]);
            x += 6;
        }
    }
}

// ------------------------------------------ //








#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "Image.h"
#include "MbedI2C.h"

#include <string>


namespace codal
{
   class BrainPadDisplay {
   
  
   uint8_t Data[2];
   //uint8_t ScreenBuffer[1024];
   int DeviceAddress = 0x78;
   typedef unsigned char byte;

     public:
     codal::_mbed::I2C i2c;
     BrainPadDisplay(Pin& sda, Pin& scl);
    
    // Initialize the display
    void InitScreen();
	void WriteScreenBuffer(uint8_t * buffer);

	void WriteCommand(int cmd);

	/*
  
    // Draw a pixel at given position
	void SetPixel(int,int);

    void drawBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	 // Draw the border of a circle
	void DrawCircle(int,int,int);

    // Draw a line from position 0 to position 1
	void DrawLine(int,int,int,int);

    // Draw the border of a rectangle at the given location
	void DrawRectangle(int,int,int,int);

    // Fill the rectangle
	void DrawFilledRectangle(int,int,int,int);

    // set dark pixel
	void OffPixel(int, int);

    // Display text by the x, y
	void DrawText(int, int, string, int, int);

	
    
    
    void ClearScreenBuffer();

   protected:

    void Pixel(int,int,bool);
    
   // need for text
	void ClearPartOfScreen(int, int, int, int);

	void DrawSymbol(int, int, char, int, int);
	*/


   };
}

#endif
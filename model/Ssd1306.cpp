#include "Ssd1306.h"

using namespace codal;

static Ssd1306 *device_instance = NULL;

Ssd1306::Ssd1306(Pin& sda, Pin& scl, int width, int height): Image(width, height), i2c(sda, scl)
    
         
{
  
  device_instance = this;
  
}

void Ssd1306::Ssd1306_command(int cmd) {

            
            Data[1] = (byte)cmd;
            i2c.write(DeviceAddress, Data, 2, 0);
}

 void Ssd1306::InitScreen()
{
                    Ssd1306_command(0xae);// SSD1306_DISPLAYOFF);                    // 0xAE
                    Ssd1306_command(0xd5);// SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
                    Ssd1306_command(0x80);                                  // the suggested ratio 0x80

                    Ssd1306_command(0xa8);// SSD1306_SETMULTIPLEX);                  // 0xA8
                    Ssd1306_command(64 - 1);

                    Ssd1306_command(0xd3);// SSD1306_SETDISPLAYOFFSET);              // 0xD3
                    Ssd1306_command(0x0);                                   // no offset
                    Ssd1306_command(0x40);// SSD1306_SETSTARTLINE | 0x0);            // line #0
                    Ssd1306_command(0x8d);// SSD1306_CHARGEPUMP);                    // 0x8D

                    Ssd1306_command(0x14);
                    

                    Ssd1306_command(0x20);// SSD1306_MEMORYMODE);                    // 0x20
                    Ssd1306_command(0x00);                                  // 0x0 act like ks0108
                    Ssd1306_command(0xa1);// SSD1306_SEGREMAP | 0x1);
                    Ssd1306_command(0xc8);// SSD1306_COMSCANDEC);


                    Ssd1306_command(0xda);// SSD1306_SETCOMPINS);                    // 0xDA
                    Ssd1306_command(0x12);
                    Ssd1306_command(0x81);// SSD1306_SETCONTRAST);                   // 0x81

                    //if (false)//vccstate == SSD1306_EXTERNALVCC)
                    //{ Ssd1306_command(0x9F); }
                    //else {
                    Ssd1306_command(0xCF);
                    //}

                    Ssd1306_command(0xd9);// SSD1306_SETPRECHARGE);                  // 0xd9

                    Ssd1306_command(0xF1);
                 
                    Ssd1306_command(0xd8);// SSD1306_SETVCOMDETECT);                 // 0xDB
                    Ssd1306_command(0x40);
                    Ssd1306_command(0xa4);//SSD1306_DISPLAYALLON_RESUME);           // 0xA4
                    Ssd1306_command(0xa6);// SSD1306_NORMALDISPLAY);                 // 0xA6

                    Ssd1306_command(0x2e);// SSD1306_DEACTIVATE_SCROLL);

                    Ssd1306_command(0xaf);// SSD1306_DISPLAYON);//--turn on oled panel


                    Ssd1306_command(0x21);// SSD1306_COLUMNADDR);
                    Ssd1306_command(0);   // Column start address (0 = reset)
                    Ssd1306_command(128 - 1); // Column end address (127 = reset)
                    Ssd1306_command(0x22);// SSD1306_PAGEADDR);
                    Ssd1306_command(0); // Page start address (0 = reset)
                    Ssd1306_command(7); // Page end address

   }

void Ssd1306::ClearScreenBuffer()
{
    for (int x=0; x<1024; x++) {
        ScreenBuffer[x]=0;
    }
    i2c.write(DeviceAddress, ScreenBuffer, 1024, 0);
}

void Ssd1306::WriteScreenBuffer()
{
    for (int y=0; y<8; y++) {
        Data[0]=0x00;
        Data[1]=0xB0+y;
       i2c.write(DeviceAddress, Data, 2, 0);     //Set GDDRAM page.
     
        Data[0]=0x40;
        i2c.write(DeviceAddress, Data, 1, 1);     //Tell controller next bytes are GDDRAM data.
       
        i2c.write(DeviceAddress, ScreenBuffer+128*y, 128, 0);
        
    }
}

void Ssd1306::ClearPartOfScreen(int x, int y, int width, int height) {
            if (x == 0 && y == 0 && width == 128 && height == 64) ClearScreenBuffer();
            for (int lx = x; lx < width + x; lx++)
                for (int ly = y; ly < height + y; ly++)
                    Pixel(lx, ly, false);
        }



void Ssd1306::Pixel(int x, int y, bool set) 
{
	  if (x >= 0 && x < 128 && y >= 0 && y < 64) {
		if (set) {
			ScreenBuffer[(x + (y / 8) * 128) + 1] |= (1 << (y % 8)); 
		}
         else 
        {
		   ScreenBuffer[(x + (y / 8) * 128) + 1] &= (~(1 << (y % 8)));
		}
    
	  }
}

void Ssd1306::SetPixel(int x, int y) 
{
    Pixel(x, y, true);
}




void Ssd1306::DrawCircle(int x0, int y0, int radius)
 {
  int x = 0, y = radius;
	int dp = 1 - radius;
	do {
		if (dp < 0)
			dp = dp + 2 * (++x) + 3;
		else
			dp = dp + 2 * (++x) - 2 * (--y) + 5;

		SetPixel(x0 + x, y0 + y);     //For the 8 octants
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

void Ssd1306::DrawRectangle(int x, int y, int width, int height) 
   {
     if (width < 0) return;
     if (height < 0) return;

     for (int i = x; i < x + width; i++) 
     {
       SetPixel(i, y);
       SetPixel(i, y + height - 1);
     }

     for (int i = y; i < y + height; i++)
     {
       SetPixel(x, i);
       SetPixel(x + width - 1, i);
     }            
       
   }

void Ssd1306::DrawFilledRectangle(int x, int y, int width, int height)
 {
            for (int lx = x; lx < width + x; lx++)
                for (int ly = y; ly < height + y; ly++)
                    SetPixel(lx, ly);
 }

void Ssd1306::DrawLine(int x1, int y1, int x2, int y2)
{
    int y,x,b,direction=1;
    float m;

    if (x1<0 && x2<0) return;
    if (x1>127 && x2>127) return;
    if (y1<0 && y2<0) return;
    if (y1>63 && y2>63) return;

    if ((x1 == x2) && (y1 == y2)) {
        SetPixel(x1,y1);
        return;
    }

    if (abs(x2-x1)<abs(y2-y1)) {
        if (y2<y1) direction =-1;
        m=((float)x2-(float)x1)/((float)y2-(float)y1);
        b=x1-m*y1+0.5;
        y=y1-direction;
        while(y!=y2) {
            y+=direction;
            x=m*y+b+0.5;
            SetPixel(x,y);
        }
    } else {
        if (x2<x1) direction =-1;
        m=((float)y2-(float)y1)/((float)x2-(float)x1);
        b=y1-m*x1+0.5;
        x=x1-direction;
        while(x!=x2) {
            x+=direction;
            y=m*x+b+0.5;
            SetPixel(x,y);
        }
    }

}

void Ssd1306::DrawSymbol(int x, int y, char letter, int HScale, int VScale) 
{
     int index = 5 * (letter - 32);

         for (int h = 0; h < 5; h++) {
             for (int hs = 0; hs < HScale; hs++) {
                 for (int v = 0; v < 8; v++) {
                     int show = (font[index + h] & (1 << v)) != 0;
                     for (int vs = 0; vs < VScale; vs++) {
                         Pixel(x + (h * HScale) + hs, y + (v * VScale) + vs, show);
                     }
                 }
             }
         }
     ClearPartOfScreen(x + 5 * HScale, y, HScale, 8 * VScale);// clear the space between characters
}

void Ssd1306::DrawText(int x, int y, string text, int HScale, int VScale) {
            int originalX = x;
            if (HScale == 0 || VScale == 0) return;
            if (text == "") return;
            
            const size_t length = sizeof(text);

            for (unsigned i = 0; i < length; i++) {
                if (text[i] >= 32) {
                    DrawSymbol(x, y, text[i], HScale, VScale);
                    x += (6 * HScale);

                }
                else {
                    if (text[i] == '\n') {
                        y += (9 * VScale);
                    }
                    if (text[i] == '\r') {
                        x = originalX;
                    }
                }
            }
        }


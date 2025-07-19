/*
  //=====================================================================
  // THX TO https://macsbug.wordpress.com/2022/11/29/esp32-8048s043/
  //=====================================================================
  //  HARD              : ESP32_8048S043C
  //  Display          : 4.3" 800x480 RGB LCD Touch GT911
  //  Dev environment   : Arduino IDE 1.8.19
  //  Board Manager    : arduino-esp32 2.0.5
  //  Board            : "ESP32S3 Dev Module"
  //  Upload Speed     : "921600"
  //  USB Mode         : "Hardware CDC and JTAG"
  //  USB CDC On Boot  : "Disable"
  //  USB Firmware MSC On Boot : "Disable"
  //  USB DFU On Boot  : "Disable"
  //  Upload Mode      : "UART0 / Hardware CDC"
  //  CPU Frequency    : "240MHz (WiFi/BT)"
  //  Core Degug Level : "None"
  //  Arduino Runs On  : "Core 1"
  //  Events Run On    : "Core 1"
  //  Rease All Flash before Sketxh Upload : "Disable"
  //--------------------------------------------------------------------
  //  Flash Mode       : "QIO 120MHz"
  //  Flash Size       : "16MB (128Mb)"
  //  Partition Scheme : "16MB Flash (2MB APP/12.5MB FATFS)"
  //  PSRAM            : "OPI PSRAM"
	//	Programmer			 : ESPTool (checked)
  //=====================================================================
*/
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
// #include <SdFat.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SPITFT.h>
// #include <Adafruit_ImageReader.h>
#include <LovyanGFX.hpp>
#include <lgfx_user/LGFX_ESP32S3_RGB_ESP32-8048S043.h>
#include "TAMC_GT911.h"

LGFX tft;
bool touched; // max  478,270
uint16_t tX, tY, bc = 0; // touch x y , bgcolor
uint16_t ltX=0, ltY=0; // touch x y , bgcolor

const uint16_t bitmap_cursor [] PROGMEM = {
	// 'arrow, 20x20px
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xd6ba, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd6ba, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd6ba, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd6ba, 0x1082, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffdf, 0xffdf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd6ba, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffdf, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xd69a, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xb596, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd6ba, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x9492, 0x0000, 0x4208, 0xffff, 0x4208, 
	0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd6ba, 0x0000, 
	0x0000, 0x94b2, 0xffff, 0xffff, 0x4208, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xd6ba, 0x0000, 0x0000, 0x0000, 0x0000, 0x94b2, 0xffff, 0xffff, 0x4208, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0xffff, 0xffff, 0xffff, 0xd6ba, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x94b2, 0xffff, 0xffff, 0x4208, 0x0841, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xd6ba, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x94b2, 0xffff, 0xffff, 0x4208, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xd6ba, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x94b2, 0xffff, 0xffff, 0x4208, 0x0841, 0x0000, 0x0000, 0x0000, 0xd6ba, 0x0841, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x94b2, 0xffff, 0xffff, 0x4208, 0x0841, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x94b2, 
	0xffff, 0xffff, 0x4208, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x94b2, 0xffff, 0xffff, 0x4208, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x94b2, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

uint8_t deltaTime=0;
double currentTime=0;
int xrec=0,yrec=0;
static LGFX_Sprite canvas(&tft);
float cX=0, cY=0, cAcc=0.03, cVel=0;
// float lcX=0, lcY=0;

// SdFat                SD;
// Adafruit_Image img;
// const uint16_t bgImage_cursor [] PROGMEM;

// /images/bliss.bmp - 24bit
// /images/arrow.png
// /images/fondo.jpg
static LGFX_Sprite background(&tft);

void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.setBrightness(255); 
  tft.setColorDepth(24);

  tft.fillScreen(TFT_DARKGREY);


	if (!SD.begin(10)) {
    Serial.println("Error al montar la tarjeta SD");
    // return;
  }
  
	uint8_t cardType = SD.cardType();
	  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

	canvas.createSprite(20, 20);
  showBMP("images/bliss.bmp", 100, 100);
}


void loop(){
  currentTime = millis();
  deltaTime=(currentTime - deltaTime)/1000;

  touched = tft.getTouch( &tX, &tY);
  if (touched) {
    if(ltX!=tX || ltY!=tY){
      tft.fillRect(ltX, ltY, 22, 22, TFT_RED);
      canvas.pushImage(0, 0, 20, 20, bitmap_cursor);

      Serial.printf("%d, %d, %d, %d, %d\n", deltaTime, tX, tY, ltX, ltY);
 
      ltX=tX;
      ltY=tY;
    }
  }
  canvas.pushSprite(ltX, ltY);
  xrec++;
  if(xrec>400){
    xrec=0;
  }
  // deltaTime=currentTime;
}


// BMP IMAGES
#define BUFFPIXEL      20
#define PALETTEDEPTH   0
uint16_t read16(File& f) {
    uint16_t result;         // read little-endian
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}
uint32_t read32(File& f) {
    uint32_t result;
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}
uint8_t showBMP(char *nm, int x, int y)
{
    File bmpFile;
    int bmpWidth, bmpHeight;    // W+H in pixels
    uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
    uint32_t bmpImageoffset;    // Start of image data in file
    uint32_t rowSize;           // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
    uint8_t bitmask, bitshift;
    boolean flip = true;        // BMP is stored bottom-to-top
    int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
    uint32_t pos;               // seek position
    boolean is565 = false;      //

    uint16_t bmpID;
    uint16_t n;                 // blocks read
    uint8_t ret;

    if ((x >= tft.width()) || (y >= tft.height()))
        return 1;               // off screen

    bmpFile = SD.open(nm);      // Parse BMP header
    bmpID = read16(bmpFile);    // BMP signature
    (void) read32(bmpFile);     // Read & ignore file size
    (void) read32(bmpFile);     // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);       // Start of image data
    (void) read32(bmpFile);     // Read & ignore DIB header size
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    n = read16(bmpFile);        // # planes -- must be '1'
    bmpDepth = read16(bmpFile); // bits per pixel
    pos = read32(bmpFile);      // format
    if (bmpID != 0x4D42) ret = 2; // bad ID
    else if (n != 1) ret = 3;   // too many planes
    else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
    else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette 
    else {
        bool first = true;
        is565 = (pos == 3);               // ?already in 16-bit format
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
        if (bmpHeight < 0) {              // If negative, image is in top-down order.
            bmpHeight = -bmpHeight;
            flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x + w) >= tft.width())       // Crop area to be loaded
            w = tft.width() - x;
        if ((y + h) >= tft.height())      //
            h = tft.height() - y;

        if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
            //bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
            bmpFile.seek(bmpImageoffset - (4<<bmpDepth)); //54 for regular, diff for colorsimportant
            bitmask = 0xFF;
            if (bmpDepth < 8)
                bitmask >>= bmpDepth;
            bitshift = 8 - bmpDepth;
            n = 1 << bmpDepth;
            lcdbufsiz -= n;
            palette = lcdbuffer + lcdbufsiz;
            for (col = 0; col < n; col++) {
                pos = read32(bmpFile);    //map palette to 5-6-5
                palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
            }
        }

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
        for (row = 0; row < h; row++) { // For each scanline...
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            uint8_t r, g, b, *sdptr;
            int lcdidx, lcdleft;
            if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
                pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            else        // Bitmap is stored top-to-bottom
                pos = bmpImageoffset + row * rowSize;
            if (bmpFile.position() != pos) { // Need seek?
                bmpFile.seek(pos);
                buffidx = sizeof(sdbuffer); // Force buffer reload
            }

            for (col = 0; col < w; ) {  //pixels in row
                lcdleft = w - col;
                if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
                for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
                    uint16_t color;
                    // Time to read more pixel data?
                    if (buffidx >= sizeof(sdbuffer)) { // Indeed
                        bmpFile.read(sdbuffer, sizeof(sdbuffer));
                        buffidx = 0; // Set index to beginning
                        r = 0;
                    }
                    switch (bmpDepth) {          // Convert pixel from BMP to TFT format
                        case 32:
                        case 24:
                            b = sdbuffer[buffidx++];
                            g = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            if (bmpDepth == 32) buffidx++; //ignore ALPHA
                            color = tft.color565(r, g, b);
                            break;
                        case 16:
                            b = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            if (is565)
                                color = (r << 8) | (b);
                            else
                                color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
                            break;
                        case 1:
                        case 4:
                        case 8:
                            if (r == 0)
                                b = sdbuffer[buffidx++], r = 8;
                            color = palette[(b >> bitshift) & bitmask];
                            r -= bmpDepth;
                            b <<= bmpDepth;
                            break;
                    }
                    lcdbuffer[lcdidx] = color;

                }
                tft.pushColors(lcdbuffer, lcdidx, first);
                first = false;
                col += lcdidx;
            }           // end cols
        }               // end rows
        tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
        ret = 0;        // good render
    }
    bmpFile.close();
    return (ret);
}
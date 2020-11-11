#ifndef IMAGELOADING_H
#define IMAGELOADING_H

#include "Arduino.h"
#include "SD.h"
#include "DisplayContainer.h"

#define SD_CS 5
#define NAMEMATCH ""         // "" matches any name
//#define NAMEMATCH "tiger"    // *tiger*.bmp
#define PALETTEDEPTH   8     // support 256-colour Palette

extern char namebuf[32];   //BMP files in root directory
//char namebuf[32] = "/bitmaps/";  //BMP directory e.g. files in /bitmaps/*.bmp

#define BMPIMAGEOFFSET 54

#define BUFFPIXEL 20

extern File root;
extern int pathlen;

uint16_t read16(File& f);
uint32_t read32(File& f);
void initializeImageLoading();
uint8_t showBMP(char *nm, int x, int y);

#endif
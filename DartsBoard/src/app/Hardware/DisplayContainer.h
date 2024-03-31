#ifndef DISPLAYCONTAINER_H
#define DISPLAYCONTAINER_H

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
//#include "Color.h"
#include "Sector.h"
#include "Pair.h"
#include <SD.h>
#include <SPI.h>

#include "../../Configuration.h"

class DisplayContainer{
	//resolution of the display
	int width, height;
	
	//corner points for touchscreen calibration
	int TL_X, TL_Y, TR_X, TR_Y, BL_X, BL_Y, BR_X, BR_Y;
	
	int A, B, C, D;
	int E, F, G, H;
	
	uint8_t xp, yp, xm, ym;
	uint16_t rx;
	
	MCUFRIEND_kbv tft;
	TouchScreen touchScreen = TouchScreen(xp, yp, xm, ym, rx);
	
	int minPressure, maxPressure;

	//image showing
	char namebuf[32]; 
    File root;
    int pathlen;
	
public:	
	DisplayContainer();
	void init();
	void setTouchCornerValues(int[4][2]);
	MCUFRIEND_kbv* getTFT() {return &tft;}
	void setSizes(int, int);
	Pair getTouchedPoint();
	void setTouchScreenPins(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
	void setPressures(int, int);
	void getCalibratedValue(int& calX, int& calY, int rawX, int rawY);
	
	//printing to display
	void write(int x, int y, int color, int size, const char* text);
	void writeRight(int a, int b, int color, int size, const char* text);

	void writeWithBackground(int x, int y, int color, int back, int size, const char* text);
	void writeCenterX(int y, int color, int backColor, int size, const char* text);
	void writeCenterY(int x, int color, int backColor, int size, const char* text);
	void writeCenter(const char* text, int size);

	static String sectorText(Sector& sector);

	uint8_t showBMP(char *nm, int x, int y);
	uint16_t read16(File& f);
	uint32_t read32(File& f);
};

#endif
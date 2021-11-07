#ifndef DISPLAYCONTAINER_H
#define DISPLAYCONTAINER_H

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "Color.h"
#include "Sector.h"
#include "Pair.h"

//corner values for touchscreen calibration
#define TL_X_V 902
#define TL_Y_V 238

#define TR_X_V 105
#define TR_Y_V 230

#define BL_X_V 908	//899
#define BL_Y_V 479	//595

#define BR_X_V 122 //108
#define BR_Y_V 585 //675

// These are the pins for the shield!
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
#define RX 300

//size of the screen
#define SCR_WIDTH 400
#define SCR_HEIGHT 240

//pressure values
#define MINPRESSURE 200
#define MAXPRESSURE 1000


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
	
public:	
	DisplayContainer() {};
	void setTouchCornerValues(int[4][2]);
	MCUFRIEND_kbv* getTFT() {return &tft;}
	void setSizes(int, int);
	Pair getTouchedPoint();
	void setTouchScreenPins(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
	void setPressures(int, int);
	void getCalibratedValue(int& calX, int& calY, int rawX, int rawY);
	
	//printing to display
	void Write(int x, int y, int color, int size, String text);
	void WriteRight(int a, int b, int color, int size,  String text);

	void WriteWithBackground(int x, int y, int color, int back, int size, String text);
	void WriteCenterX(int y, int color, int backColor, int size, String text);
	void WriteCenterY(int x, int color, int backColor, int size, String text);
	void WriteCenter(String text, int size);
	
	
	static DisplayContainer displayContainer;
	static void Initialize();

	static String SectorText(Sector sector);
};

#endif
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//corner values for touchscreen calibration
#define TL_X_V 902
#define TL_Y_V 238

#define TR_X_V 105
#define TR_Y_V 230

#define BL_X_V 908	//899
#define BL_Y_V 479	//595

#define BR_X_V 122 //108
#define BR_Y_V 585 //675

/*#define TL_X_V 912
#define TL_Y_V 225

#define TR_X_V 120
#define TR_Y_V 230

#define BL_X_V 920	//899
#define BL_Y_V 424	//595

#define BR_X_V 122 //108
#define BR_Y_V 567 //675*/

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

//image loading
#define BMPIMAGEOFFSET 54
#define BUFFPIXEL 20
#define SD_CS 5
#define NAMEMATCH ""         // "" matches any name
#define PALETTEDEPTH   8     // support 256-colour Palette

#endif
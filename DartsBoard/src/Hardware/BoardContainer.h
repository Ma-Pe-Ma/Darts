#ifndef BOARDCONTAINER_H
#define BOARDCONTAINER_H

#define DROWS 8
#define DCOLS 8

#include "Sector.h"
#include <Keypad.h>

class BoardContainer{
	//switches for correct pin ordering
	bool boardSetup[3] = {1,1,1};

	const char dKeys[DROWS][DCOLS]  = {
		{'%','6','J','0','j','a','1','A'},
		{'Z','Y','O','\'','o','t',',','T'},
		{'m','f','B','2','b','e','5','E'},
		{'d','r','Q',')','q','l','$','L'},
		{'4','*','C','3','c','i','9','I'},
		{'D','R','S','+','s','n','&','N'},
		{'M','F','G','7','g','k','#','K'},
		{'.','-','P','(','p','h','8','H'},
	};

	byte dRowPins[DROWS] = {38, 40, 42, 44, 46, 48, 50, 52};
	byte dColPins[DCOLS] = {39, 41, 43, 45, 47, 49, 51, 53};

	byte setupDRP[DROWS];
	byte setupDCP[DCOLS];

	void setupCorrectPinOrder();
public: 
	BoardContainer();
	Keypad* dartsBoard;
	static uint8_t currentDart;
	static Sector darts[3];
	static Sector sectorMapping(char x);
	static BoardContainer boardContainer;
};


#endif
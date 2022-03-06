#ifndef BOARDCONTAINER_H
#define BOARDCONTAINER_H

#define DROWS 8
#define DCOLS 8

#include "Sector.h"
#include <Keypad.h>

/* My board's matrix:
S13 S06 D10 S10 T10 T01 S01 D01
D25 S25 D15 S15 T15 T20 S20 D20
T13 T06 D02 S02 T02 T05 S05 D05
T04 T18 D17 S17 T17 T12 S12 D12
S04 S18 D03 S03 T03 T09 S09 D09
D04 D18 D19 S19 T19 T14 S14 D14
D13 D06 D07 S07 T07 T11 S11 D11
--- --- D14 S14 T14 T08 S08 D08*/

class BoardContainer{
	//switches for correct pin ordering
	bool boardSetup[3] = {1,1,1};

	Keypad* dartsBoard;
	const char dKeys[DROWS][DCOLS]  = {
		{13, 46, 10, 20, 30, 41,  1, 21},
		{62, 62, 35, 15, 55, 60, 20, 40},
		{53, 46, 22,  2, 42, 45,  5, 25},
		{44, 58, 37, 17, 57, 52, 12, 32},
		{ 4, 18, 23,  3, 43, 49,  9, 29},
		{24, 38, 39, 19, 59, 54, 14, 34},
		{33, 26, 27,  7, 47, 51, 11, 31},
		{63, 64, 34, 14, 54, 48,  8, 28},
	};

	byte dRowPins[DROWS] = {38, 40, 42, 44, 46, 48, 50, 52};
	byte dColPins[DCOLS] = {39, 41, 43, 45, 47, 49, 51, 53};

	byte setupDRP[DROWS];
	byte setupDCP[DCOLS];

	void setupCorrectPinOrder();

	uint8_t currentDartID = 0;
	Sector darts[3];
public: 
	BoardContainer();
	Sector sectorMapping(char x);

	uint8_t getCurrentDartID() {
		return this->currentDartID;
	}

	void setCurrentDartID(uint8_t currentDartID) {
		this->currentDartID = currentDartID;
	}

	Sector getThrownDartByNumber(int i) {
		return this->darts[i];
	}

	void setThrownDartByNumber(int i, Sector dart) {
		this->darts[i] = dart;
	}

	Keypad* getDartsBoard() {
		return this->dartsBoard;
	}
};

#endif
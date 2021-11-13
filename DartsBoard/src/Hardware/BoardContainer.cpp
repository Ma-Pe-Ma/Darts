#include "BoardContainer.h"

BoardContainer::BoardContainer() {
	setupCorrectPinOrder();
	dartsBoard = new Keypad(makeKeymap(dKeys), setupDRP, setupDCP, DROWS, DCOLS);	
	dartsBoard->setDebounceTime(200);
	dartsBoard->setHoldTime(500);
}

void BoardContainer::setupCorrectPinOrder() {
	for(int i = 0; i < 8; i++) {
		if(boardSetup[0]) {
			setupDRP[7 - i] = dRowPins[i];
		}			
		else {
			setupDRP[i] = dRowPins[i];
		}
		
		if(boardSetup[1]) {
			setupDCP[7 - i] = dColPins[i];
		}
		else {
			setupDCP[i] = dColPins[i];
		}	
	}
	
	if(boardSetup[2]) {
		for(int i = 0; i < 8; i++) {
			int temp = setupDRP[i];
			setupDRP[i] = setupDCP[i];
			setupDCP[i] = temp;
		}
	}
}

Sector BoardContainer::sectorMapping(char x) {
	Sector dart;
	
	//Simple numbers from 1-9
	if(x > 48 && x < 58) {
		dart.multiplier = 1;
		dart.base = x - 48;
	}
	
	//Simple 10
	if(x == 48) {
		dart.multiplier = 1;
		dart.base = 10;
	}
	
	//Simple Numbers from 11-20
	if(x > 34 && x < 45) {
		dart.multiplier = 1;
		dart.base = x - 24;	
	}
	
	//Double numbers from 1-20
	if(x > 64 && x < 85) {
		dart.multiplier = 2;
		dart.base = x - 64;	
	}
	
	//Triple numbers from 1-20
	if(x > 96 && x < 117) {
		dart.multiplier = 3;
		dart.base = x - 96;	
	}		
	
	//Simple Bulls Eye
	if(x == 89) {
		dart.multiplier = 1;
		dart.base = 25;	
	}		
	
	//Double Bull's Eye
	if(x == 90) {
		dart.multiplier = 2;
		dart.base = 25;	
	}
	
	return dart;
}
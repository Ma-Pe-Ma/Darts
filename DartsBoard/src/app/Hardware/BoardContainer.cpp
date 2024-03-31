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
	
	if (x < 61) {
		dart.base = (x - 1) % 20 + 1;
		dart.multiplier = (x - 1) / 20 + 1;
	}
	else {
		dart.base = 25;
		dart.multiplier = x % 60 == 1 ? 1 : 2;
	}
	
	return dart;
}
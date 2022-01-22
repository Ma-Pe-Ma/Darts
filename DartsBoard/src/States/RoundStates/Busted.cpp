#include "Busted.h"

#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

void Busted::update(Pair touch) {
    
    if (millis() - timer > 1000) {
		int dartID = gamePlayingScreen->boardContainer.getCurrentDartID();
		Sector dart = gamePlayingScreen->boardContainer.getThrownDartByNumber(dartID);

		//Write to dart list
		String text = String(dartID + 1) + ": " + DisplayContainer::sectorText(dart);
		gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * dartID, dartStatusStartY, BLACK, CYAN, 2, text);
		
		//Draw Status
		Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();
		currentPlayer->getScore()->status();

		int newDartID = ++dartID;
		gamePlayingScreen->boardContainer.setCurrentDartID(newDartID);

		gamePlayingScreen->transitionTo(&gamePlayingScreen->outro);
	}
}
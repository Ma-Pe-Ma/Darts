#include "Hit.h"
#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

void Hit::start() {
	int x = SCR_WIDTH / 8;
	int y = SCR_WIDTH / 8;
	int textSize = 7;

	int dartID = gamePlayingScreen->boardContainer.getCurrentDartID();
	Sector dart = gamePlayingScreen->boardContainer.getThrownDartByNumber(dartID);
	String text = String(dartID + 1) + ": " + DisplayContainer::sectorText(dart);

	Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();

	//Write current hit
	gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(x, y, TFT_BLACK, TFT_CYAN, textSize, "       ");	
	gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(x, y, TFT_BLACK, currentPlayer->getColor(), textSize, text.c_str());

	gamePlayingScreen->sendHit(dart);
	
	currentPlayer->getScore()->statusAfterHit(dart);
	timer = millis();

	//playaudio
}

void Hit::update(Pair pair) {
    if (millis() - timer > 1000) {
		int dartID = gamePlayingScreen->boardContainer.getCurrentDartID();
		Sector dart = gamePlayingScreen->boardContainer.getThrownDartByNumber(dartID);

		//Write to dart list
		String text = String(dartID + 1) + ": " + DisplayContainer::sectorText(dart);
		gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * dartID, dartStatusStartY, TFT_BLACK, TFT_CYAN, 2, text.c_str());
		
		//Draw Status
		Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();
		currentPlayer->getScore()->status();

		int newDartID = ++dartID;
		gamePlayingScreen->boardContainer.setCurrentDartID(newDartID);

		if (newDartID < 3) {
			gamePlayingScreen->transitionTo(&gamePlayingScreen->throwing);
		}
		else {
			gamePlayingScreen->transitionTo(&gamePlayingScreen->outro);
		}
	}
}
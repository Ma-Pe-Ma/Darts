#include "Hit.h"
#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

void Hit::start() {
	digitalWrite(22, HIGH);

	int x = SCR_WIDTH / 8;
	int y = SCR_WIDTH / 8;
	int textSize = 7;

	int dartID = gamePlayingScreen->boardContainer.getCurrentDartID();
	Sector dart = gamePlayingScreen->boardContainer.getThrownDartByNumber(dartID);
	String text = String(dartID + 1) + ": " + DisplayContainer::sectorText(dart);

	Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();

	gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(x, y, BLACK, CYAN, textSize, "       ");	
	gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(x, y, BLACK, currentPlayer->getColor(), textSize, text);

	gamePlayingScreen->sendHit(dart);
	currentPlayer->getScore()->statusAfterHit(dart);
	timer = millis();
}

void Hit::update(Pair pair) {
    if (millis() - timer > 1000) {
		digitalWrite(22, LOW);

		int dartID = gamePlayingScreen->boardContainer.getCurrentDartID();
		Sector dart = gamePlayingScreen->boardContainer.getThrownDartByNumber(dartID);

		String text = String(dartID + 1) + ": " + DisplayContainer::sectorText(dart);
		gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * dartID, dartStatusStartY, BLACK, CYAN, 2, text);
		
		Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();
		currentPlayer->getScore()->status();

		int newDartID = dartID++;
		gamePlayingScreen->boardContainer.setCurrentDartID(newDartID);

		if (newDartID == 2) {
			gamePlayingScreen->transitionTo(&gamePlayingScreen->outro);
		}
		else {
			gamePlayingScreen->transitionTo(&gamePlayingScreen->throwing);
		}
	}
}
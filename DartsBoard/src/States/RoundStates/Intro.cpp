#include "Intro.h"
#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

void Intro::start() {
	gamePlayingScreen->getGameLogic()->playerContainer->findNextPlayer();
	Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();

	currentPlayer->getScore()->setRoundCounter(gamePlayingScreen->roundCounter);
	currentPlayer->getScore()->drawCompleteCustomStatus();

	//Write out player changing!
	String roundText = "R" + String(gamePlayingScreen->roundCounter) + "-P" + String(gamePlayingScreen->getGameLogic()->playerContainer->getPlayerCursor() + 1);
	if (gamePlayingScreen->roundCounter < 100) {
		roundText += "";
	}
	int textSize = 7;
	gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(SCR_WIDTH / 8, SCR_WIDTH / 8, WHITE, BLACK, textSize, roundText);
	
	//delete thrown dart data
	gamePlayingScreen->boardContainer.setCurrentDartID(0);

	for(int i = 0; i < 3; i++) {
		gamePlayingScreen->boardContainer.setThrownDartByNumber(i, {.base = 0, .multiplier = 0});
		gamePlayingScreen->del[i] = false;
	}

	//delete the thrown dart texts
	int dartStatusStartX = SCR_WIDTH / 10;
	int dartStatusOffsetX = int(SCR_WIDTH * 0.333f);

	int dartStatusStartY = SCR_HEIGHT / 2;

	for (int i = 0; i < 3; i++) {
		Sector dart = gamePlayingScreen->boardContainer.getThrownDartByNumber(i);
		gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * i, dartStatusStartY, BLACK, CYAN, 2, String(i + 1) + ": " + DisplayContainer::sectorText(dart));
	}		
	
	/*for (int i = 0; i < LIST_MAX; i++) {
		dartsBoard->key[i].kstate = IDLE;
	}*/

	gamePlayingScreen->sendRoundDump();
	timer = millis();
}

void Intro::update(Pair pair) {
	Serial.println("Intro update!");
    if (millis() - timer > 1000) {
		gamePlayingScreen->transitionTo(&gamePlayingScreen->throwing);
		//Serial.println("Transitioned to throwing");
		Serial.println("Transition to throw!!");
	}
}
#include "Outro.h"
#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

void Outro::start() {
	int cursor = gamePlayingScreen->getGameLogic()->playerContainer->getPlayerCursor();
	Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();
	text = "P" + String(cursor + 1) + "-" + currentPlayer->getScore()->getPlayerScore();
}

void Outro::update(Pair pair) {
	if ((millis() / 1000) % 2 == 0) {
		Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();
		gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(x, y, currentPlayer->getInverseColor(), currentPlayer->getColor(), textSize, text.c_str());
	}		
	else {
		Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();
		gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(x, y, currentPlayer->getColor(), currentPlayer->getInverseColor(), textSize, text.c_str());
	}
	
	if (gamePlayingScreen->getGameLogic()->nextMenu.simple()) {
		gamePlayingScreen->getGameLogic()->nextMenu.guiButton.drawButton(false);
		gamePlayingScreen->transitionTo(&gamePlayingScreen->intro);
	}
}
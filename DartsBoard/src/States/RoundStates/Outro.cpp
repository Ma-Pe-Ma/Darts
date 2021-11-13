#include "Outro.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

void Outro::start() {
	text = "P" + String(Player::getPlayerCursor() + 1) + "-" + Player::getCurrentPlayer()->getScore()->getPlayerScore();
}

void Outro::update(Pair pair) {
	if ((millis() / 1000) % 2 == 0) {
		Player* currentPlayer = Player::getCurrentPlayer();
		DisplayContainer::displayContainer.writeWithBackground(x, y, currentPlayer->getInverseColor(), currentPlayer->getColor(), textSize, text);	
	}		
	else {
		Player* currentPlayer = Player::getCurrentPlayer();
		DisplayContainer::displayContainer.writeWithBackground(x, y, currentPlayer->getColor(), currentPlayer->getInverseColor(), textSize, text);	
	}
	
	if (gamePlayingScreen->getGameLogic()->nextMenu.simple()) {
		gamePlayingScreen->getGameLogic()->nextMenu.guiButton.drawButton(false);
		gamePlayingScreen->transitionTo(&gamePlayingScreen->intro);
	}
}
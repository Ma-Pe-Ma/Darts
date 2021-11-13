#include "Hit.h"
#include "../GamePlayingScreen.h"

void Hit::start() {
	digitalWrite(22, HIGH);

	int x = SCR_WIDTH / 8;
	int y = SCR_WIDTH / 8;
	int textSize = 7;
	String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::sectorText(BoardContainer::darts[BoardContainer::currentDart]);

	DisplayContainer::displayContainer.writeWithBackground(x, y, BLACK, CYAN, textSize, "       ");	
	DisplayContainer::displayContainer.writeWithBackground(x, y, BLACK, Player::getCurrentPlayer()->getColor(), textSize, text);

	gamePlayingScreen->sendHit(BoardContainer::darts[BoardContainer::currentDart]);
	Player::getCurrentPlayer()->getScore()->statusAfterHit(BoardContainer::darts[BoardContainer::currentDart]);
	timer = millis();
}

void Hit::update(Pair pair) {
    if (millis() - timer > 1000) {
		digitalWrite(22, LOW);

		String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::sectorText(BoardContainer::darts[BoardContainer::currentDart]);
		DisplayContainer::displayContainer.writeWithBackground(dartStatusStartX + dartStatusOffsetX * BoardContainer::currentDart, dartStatusStartY, BLACK, CYAN, 2, text);
		
		Player::getCurrentPlayer()->getScore()->status();

		if (BoardContainer::boardContainer.currentDart++ == 2) {
			gamePlayingScreen->transitionTo(&gamePlayingScreen->outro);
		}
		else {
			gamePlayingScreen->transitionTo(&gamePlayingScreen->throwing);
		}
	}
}
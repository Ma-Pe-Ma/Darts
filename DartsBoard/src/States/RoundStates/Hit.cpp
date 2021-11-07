#include "Hit.h"
#include "../GamePlayingScreen.h"

void Hit::Start() {
	digitalWrite(22, HIGH);

	int x = SCR_WIDTH / 8;
	int y = SCR_WIDTH / 8;
	int textSize = 7;
	String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[BoardContainer::currentDart]);

	DisplayContainer::displayContainer.WriteWithBackground(x, y, BLACK, CYAN, textSize, "       ");	
	DisplayContainer::displayContainer.WriteWithBackground(x, y, BLACK, Player::current->color, textSize, text);

	gamePlayingScreen->SendHit(BoardContainer::darts[BoardContainer::currentDart]);
	Player::current->score->StatusAfterHit(BoardContainer::darts[BoardContainer::currentDart]);
	timer = millis();
}

void Hit::Update(Pair pair) {
    if (millis() - timer > 1000) {
		digitalWrite(22, LOW);

		String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[BoardContainer::currentDart]);
		DisplayContainer::displayContainer.WriteWithBackground(dartStatusStartX + dartStatusOffsetX * BoardContainer::currentDart, dartStatusStartY, BLACK, CYAN, 2, text);
		
		Player::current->score->AbstractScore::Status();

		if (BoardContainer::boardContainer.currentDart++ == 2) {
			gamePlayingScreen->TransitionTo(&gamePlayingScreen->outro);
		}
		else {
			gamePlayingScreen->TransitionTo(&gamePlayingScreen->throwing);
		}
	}
}
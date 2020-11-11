#include "Hit.h"
#include "../GamePlayingScreen.h"

void Hit::Start() {
	digitalWrite(22, HIGH);

	int x = 50;
	int y = 50;
	int s = 7;
	String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[BoardContainer::currentDart]);

	DisplayContainer::displayContainer.WriteWithBackground(x, y, BLACK, CYAN, s, "       ");	
	DisplayContainer::displayContainer.WriteWithBackground(x, y, BLACK, Player::current->color, s, text);

	gamePlayingScreen->SendHit(BoardContainer::darts[BoardContainer::currentDart]);
	Player::current->score->StatusAfterHit(BoardContainer::darts[BoardContainer::currentDart]);
	timer = millis();
}

void Hit::Update(Pair pair) {
    if (1000 < millis() - timer) {
		digitalWrite(22, LOW);
		
		int x = 50;
		int y = 50;		
		int s = 7;
		String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[BoardContainer::currentDart]);
		DisplayContainer::displayContainer.WriteWithBackground(40 + 130 * BoardContainer::currentDart, 120, BLACK, CYAN, 2, text);
		
		Player::current->score->AbstractScore::Status();

		if (BoardContainer::boardContainer.currentDart++ == 2) {
			gamePlayingScreen->TransitionTo(&gamePlayingScreen->outro);
		}
		else {
			gamePlayingScreen->TransitionTo(&gamePlayingScreen->throwing);
		}
	}
}
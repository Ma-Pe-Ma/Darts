#include "Intro.h"
#include "../GamePlayingScreen.h"

void Intro::Start() {
	int s = 7;
	int x = 50;
	int y = 50;

	Player::FindNextPlayer();
	Player::current->score->roundCounter = gamePlayingScreen->roundCounter;
	Player::current->score->DrawCompleteCustomStatus();

	//Write out player changing!
	String roundText = "R" + String(gamePlayingScreen->roundCounter) + "-P" + String(Player::cursor + 1);
	if (gamePlayingScreen->roundCounter < 100) {
		roundText += "";
	}
	DisplayContainer::displayContainer.WriteWithBackground(x,y, WHITE, BLACK, s, roundText);

	//initialize game specific settings
	DartsGame::dartsGame->InitializeRound();
	
	//delete thrown dart data
	BoardContainer::boardContainer.currentDart = 0;

	for(int i = 0; i < 3; i++) {
		BoardContainer::darts[i].multiplier = 0;
		BoardContainer::darts[i].base = 0;
		gamePlayingScreen->del[i] = false;
	}

	//delete the thrown dart texts
	for (int i = 0; i < 3; i++) {
		DisplayContainer::displayContainer.WriteWithBackground(40 + 130 * i, 120, BLACK, CYAN, 2, "" + String(i + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[i]));
	}		
	
	/*for (int i = 0; i < LIST_MAX; i++) {
		dartsBoard->key[i].kstate = IDLE;
	}*/

	if (firstTime == true) {
		firstTime = false;
	}
	else {
		gamePlayingScreen->SendRoundDump();
	}
	

	timer = millis();
}

void Intro::Update(Pair pair) {
    if (1000 < millis() - timer) {
		gamePlayingScreen->TransitionTo(&gamePlayingScreen->throwing);
		Serial.println("Transitioned to throwing");
	}
}
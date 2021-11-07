#include "Intro.h"
#include "../GamePlayingScreen.h"

void Intro::Start() {
	Player::FindNextPlayer();
	Player::current->score->roundCounter = gamePlayingScreen->roundCounter;
	Player::current->score->DrawCompleteCustomStatus();

	//Write out player changing!
	String roundText = "R" + String(gamePlayingScreen->roundCounter) + "-P" + String(Player::cursor + 1);
	if (gamePlayingScreen->roundCounter < 100) {
		roundText += "";
	}
	int textSize = 7;
	DisplayContainer::displayContainer.WriteWithBackground(SCR_WIDTH / 8, SCR_WIDTH / 8, WHITE, BLACK, textSize, roundText);

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
	int dartStatusStartX = SCR_WIDTH / 10;
	int dartStatusOffsetX = int(SCR_WIDTH * 0.333f);

	int dartStatusStartY = SCR_HEIGHT / 2;

	for (int i = 0; i < 3; i++) {
		DisplayContainer::displayContainer.WriteWithBackground(dartStatusStartX + dartStatusOffsetX * i, dartStatusStartY, BLACK, CYAN, 2, String(i + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[i]));
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
    if (millis() - timer > 1000) {
		gamePlayingScreen->TransitionTo(&gamePlayingScreen->throwing);
		//Serial.println("Transitioned to throwing");
	}
}
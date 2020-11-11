#include "Outro.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

void Outro::Start() {
	text = "P" + String(Player::cursor + 1) + "-" + Player::current->score->playerScore;
}

void Outro::Update(Pair pair) {
	if ((millis() / 1000) % 2 == 0) {
		DisplayContainer::displayContainer.WriteWithBackground(x, y, Player::current->inverseColor, Player::current->color, s, text);	
	}		
	else {
		DisplayContainer::displayContainer.WriteWithBackground(x, y, Player::current->color, Player::current->inverseColor, s, text);	
	}
	
	if (gamePlayingScreen->GetGameLogic()->nextMenu.simple()) {
		gamePlayingScreen->GetGameLogic()->nextMenu.guiButton.drawButton(false);
		gamePlayingScreen->TransitionTo(&gamePlayingScreen->intro);
	}
}
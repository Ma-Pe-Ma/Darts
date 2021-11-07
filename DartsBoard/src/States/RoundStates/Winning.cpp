#include "Winning.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

void Winning::Start() {
	Player::current->score->position = gamePlayingScreen->nextWinningPosition++;
	
	//check if game is finished
	if (Player::number == 1) {
		gamePlayingScreen->gameFinished = true;
	}
	else if (gamePlayingScreen->nextWinningPosition == Player::number) {
		gamePlayingScreen->gameFinished = true;

		//set the remaining player's position to last!
		for (int i = 0; i < Player::number; i++) {
			if (Player::players[i].score->position == -1) {
				Player::players[i].score->position = Player::number;
				break;
			}
		}
	}

	DisplayContainer::displayContainer.getTFT()->fillScreen(WHITE);

	String winnerText;

	String nick = Player::current->nickname;
	if (nick == "") {
		nick = "P" + String(Player::current->ID + 1);
	}

	if (Player::current->score->position == 1) {
		winnerText = "Winner: " + nick;
	} 
	else {
		winnerText = String(Player::current->score->position) + ". pos: " + nick;
	}
	
	DisplayContainer::displayContainer.WriteCenterX(SCR_HEIGHT / 5, Player::current->color, Player::current->inverseColor, 4, winnerText);

	gamePlayingScreen->SendDump();

	timer = millis();
}

void Winning::Update(Pair pair) {
    if(millis() - timer > winningTime * 1000) {
		if (gamePlayingScreen->gameFinished) {
			if (Player::number == 1) {
				gameLogic->TransitionTo(&gameLogic->gameConfiguringScreen);
			}
			else {
				gamePlayingScreen->TransitionTo(&gamePlayingScreen->postConfig);
			}
		}
		else {
			DisplayContainer::displayContainer.getTFT()->fillScreen(CYAN);
			gameLogic->prevMenu.guiButton.drawButton(true);
			gameLogic->nextMenu.guiButton.drawButton(true);

			int dartStatusStartX = SCR_WIDTH / 10;
			int dartStatusOffsetX = int(SCR_WIDTH * 0.333f);

			int dartStatusStartY = SCR_HEIGHT / 2;

			//redraw status
			for (int i = 0; i < 3; i++) {
				DisplayContainer::displayContainer.WriteWithBackground(dartStatusStartX + dartStatusOffsetX * i, dartStatusStartY, BLACK, CYAN, 2, String(i + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[i]));
			}
			Player::current->score->DrawCompleteCustomStatus();

			gamePlayingScreen->TransitionTo(&gamePlayingScreen->outro);
		}
	}
}
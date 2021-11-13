#include "Winning.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

void Winning::start() {
	Player* currentPlayer = Player::getCurrentPlayer();
	int alreadyFinishedPlayers = Player::getNumberOfFinishedPlayers();
	currentPlayer->getScore()->setPosition(++alreadyFinishedPlayers);
	
	//check if game is finished
	if (Player::getNumberOfPlayers() > 1 && alreadyFinishedPlayers == Player::getNumberOfPlayers() - 1) {
		//set the remaining player's position to last!
		for (int i = 0; i < Player::getNumberOfPlayers(); i++) {
			Player* checkable = Player::getPlayerByNumber(i);

			if (checkable->getScore()->getPosition() == -1) {
				checkable->getScore()->setPosition(Player::getNumberOfPlayers());
				break;
			}
		}
	}

	DisplayContainer::displayContainer.getTFT()->fillScreen(WHITE);

	String winnerText;

	String nick = currentPlayer->getNick();
	if (nick == "") {
		nick = "P" + String(currentPlayer->getID() + 1);
	}

	if (currentPlayer->getScore()->getPosition() == 1) {
		winnerText = "Winner: " + nick;
	} 
	else {
		winnerText = String(currentPlayer->getScore()->getPosition()) + ". pos: " + nick;
	}
	
	DisplayContainer::displayContainer.writeCenterX(SCR_HEIGHT / 5, currentPlayer->getColor(), currentPlayer->getInverseColor(), 4, winnerText);

	gamePlayingScreen->sendDump();
	timer = millis();
}

void Winning::update(Pair pair) {
    if(millis() - timer > winningTime * 1000) {
		if (Player::getNumberOfPlayers() == Player::getNumberOfFinishedPlayers()) {
			if (Player::getNumberOfPlayers() == 1) {
				gameLogic->transitionTo(&gameLogic->gameConfiguringScreen);
			}
			else {
				gamePlayingScreen->transitionTo(&gamePlayingScreen->postConfig);
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
				DisplayContainer::displayContainer.writeWithBackground(dartStatusStartX + dartStatusOffsetX * i, dartStatusStartY, BLACK, CYAN, 2, String(i + 1) + ": " + DisplayContainer::sectorText(BoardContainer::darts[i]));
			}
			Player::getCurrentPlayer()->getScore()->drawCompleteCustomStatus();

			gamePlayingScreen->transitionTo(&gamePlayingScreen->outro);
		}
	}
}
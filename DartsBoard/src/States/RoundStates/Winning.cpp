#include "Winning.h"
#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

void Winning::start() {	
	Player* currentPlayer = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer();
	int alreadyFinishedPlayers = gamePlayingScreen->getGameLogic()->playerContainer->getNumberOfFinishedPlayers();
	currentPlayer->getScore()->setPosition(++alreadyFinishedPlayers);
	
	int numberOfPlayers = gamePlayingScreen->getGameLogic()->playerContainer->getNumberOfPlayers();

	//check if game is finished
	if (numberOfPlayers > 1 && alreadyFinishedPlayers == numberOfPlayers - 1) {
		//set the remaining player's position to last!
		for (int i = 0; i < numberOfPlayers; i++) {
			Player* checkable = gamePlayingScreen->getGameLogic()->playerContainer->getPlayerByNumber(i);

			if (checkable->getScore()->getPosition() == -1) {
				checkable->getScore()->setPosition(numberOfPlayers);
				break;
			}
		}
	}

	gamePlayingScreen->getGameLogic()->displayContainer->getTFT()->fillScreen(TFT_WHITE);

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
	
	gamePlayingScreen->getGameLogic()->displayContainer->writeCenterX(SCR_HEIGHT / 5, currentPlayer->getColor(), currentPlayer->getInverseColor(), 4, winnerText.c_str());

	gamePlayingScreen->sendDump();
	timer = millis();
}

void Winning::update(Pair pair) {
    if(millis() - timer > winningTime * 1000) {
		int numberOfPlayers = gamePlayingScreen->getGameLogic()->playerContainer->getNumberOfPlayers();
		int finishedPlayers = gamePlayingScreen->getGameLogic()->playerContainer->getNumberOfFinishedPlayers();

		if (numberOfPlayers == finishedPlayers) {
			if (numberOfPlayers == 1) {
				gameLogic->transitionTo(&gameLogic->gameConfiguringScreen);
			}
			else {
				gamePlayingScreen->transitionTo(&gamePlayingScreen->postConfig);
			}
		}
		else {
			gamePlayingScreen->getGameLogic()->displayContainer->getTFT()->fillScreen(TFT_CYAN);
			gameLogic->prevMenu.guiButton.drawButton(true);
			gameLogic->nextMenu.guiButton.drawButton(true);

			int dartStatusStartX = SCR_WIDTH / 10;
			int dartStatusOffsetX = int(SCR_WIDTH * 0.333f);

			int dartStatusStartY = SCR_HEIGHT / 2;

			//redraw status
			for (int i = 0; i < 3; i++) {
				Sector dart = gamePlayingScreen->boardContainer.getThrownDartByNumber(i);
				gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * i, dartStatusStartY, TFT_BLACK, TFT_CYAN, 2, (String(i + 1) + ": " + DisplayContainer::sectorText(dart)).c_str());
			}

			gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer()->getScore()->drawCompleteCustomStatus();
			gamePlayingScreen->transitionTo(&gamePlayingScreen->outro);
		}
	}
}
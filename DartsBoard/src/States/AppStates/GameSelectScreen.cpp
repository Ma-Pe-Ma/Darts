#include "GameSelectScreen.h"
#include "../GameLogic.h"

void GameSelectScreen::initialize() {
	gameSchemeString = Resources::getTextByID(Resources::Text::gameScheme);
}

void GameSelectScreen::start() {
    getGameLogic()->displayContainer->getTFT()->fillScreen(TFT_GREEN);
	
	int textSize = 3;
	int squareOffset = int(SCR_WIDTH * 0.08f);
	int buttonSize = int(SCR_WIDTH * 0.1f);

	int yPos = int(SCR_HEIGHT * 0.666f);

	//Write choosing + chosen game name
	DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();

    getGameLogic()->displayContainer->write(int(SCR_WIDTH * 0.0625f), int(SCR_HEIGHT * 0.40f), TFT_RED, textSize, gameSchemeString);
    getGameLogic()->displayContainer->writeCenterX(yPos - textSize * 6 / 2, TFT_RED, TFT_GREEN, textSize, "               ");
    getGameLogic()->displayContainer->writeCenterX(yPos - textSize * 6 / 2, TFT_RED, TFT_GREEN, textSize, currentGame->getName().c_str());
    
	//draw game chooser buttons
	gameLogic->prevCursor.setImage(getGameLogic()->displayContainer->getTFT(), squareOffset, yPos, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, "<", 2);
    gameLogic->nextCursor.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH -squareOffset, yPos, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, ">", 2);
	
    gameLogic->prevCursor.guiButton.drawButton(true);
    gameLogic->nextCursor.guiButton.drawButton(true);
    
	//draw menu cursor buttons
    gameLogic->prevMenu.setImage(getGameLogic()->displayContainer->getTFT(), squareOffset, squareOffset, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, ">>", 2);
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);
}

void GameSelectScreen::update(Pair touch) {
    //touch detection
	gameLogic->prevCursor.detect(touch);
	gameLogic->nextCursor.detect(touch);
	
	gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);
	
	DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();

	//decrementing gametype
    if (gameLogic->prevCursor.simple()) {
		gameLogic->prevCursor.guiButton.drawButton(false);
		
		int textSize = 3;
		int yPos = int(SCR_HEIGHT * 0.666f);
		yPos -= textSize * 6 / 2;

		currentGame = gameLogic->gameContainer->prevGame();
		gameLogic->gameContainer->setCurrentGame(currentGame);
		getGameLogic()->displayContainer->writeCenterX(yPos, TFT_RED, TFT_GREEN, textSize, "               ");
		getGameLogic()->displayContainer->writeCenterX(yPos, TFT_RED, TFT_GREEN, textSize, currentGame->getName().c_str());
	}
	
	//incrementing gametype
	if (gameLogic->nextCursor.simple()) {
		gameLogic->nextCursor.guiButton.drawButton(false);

		int textSize = 3;
		int yPos = int(SCR_HEIGHT * 0.666f);
		yPos -= textSize * 6 / 2;
		
		currentGame = gameLogic->gameContainer->nextGame();
		gameLogic->gameContainer->setCurrentGame(currentGame);
		getGameLogic()->displayContainer->writeCenterX(yPos, TFT_RED, TFT_GREEN, textSize, "               ");
		getGameLogic()->displayContainer->writeCenterX(yPos, TFT_RED, TFT_GREEN, textSize, currentGame->getName().c_str());
	}

	//visualization
	if (gameLogic->prevCursor.released()) {
		gameLogic->prevCursor.guiButton.drawButton(true);
	}
	
	if (gameLogic->nextCursor.released()) {
		gameLogic->nextCursor.guiButton.drawButton(true);
	}

	//back to player config
	if (gameLogic->prevMenu.simple()) {
		gameLogic->transitionTo(&gameLogic->playerScreen);
	}
	
	//next to game config
	if (gameLogic->nextMenu.simple()) {		
		if (gameLogic->playerContainer->getNumberOfPlayers() == 0) {
			gameLogic->transitionTo(&gameLogic->playerScreen);
		}
		else {
			gameLogic->transitionTo(&gameLogic->gameConfiguringScreen);
		}
	}
}

void GameSelectScreen::processMessage(JsonObject message) {
    
}
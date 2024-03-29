#include "GameConfiguringScreen.h"
#include "../GameLogic.h"

void GameConfiguringScreen::initialize() {
	settingsString = Resources::getTextByID(Resources::Text::config);
}

void GameConfiguringScreen::start() {
    getGameLogic()->displayContainer->getTFT()->fillScreen(TFT_CYAN);
	
	int buttonSize = SCR_WIDTH / 10;
	int squareOffset = int(SCR_WIDTH * 0.08f);

    gameLogic->prevMenu.setImage(getGameLogic()->displayContainer->getTFT(), squareOffset, squareOffset, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, ">>", 2);
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);

	DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();

	String text = currentGame->getName() + settingsString;
	int textSize = 2;
	int yHeight = int(SCR_HEIGHT * 0.1f);

	getGameLogic()->displayContainer->writeCenterX(yHeight, TFT_BLACK, TFT_CYAN, textSize, text.c_str());
    
    currentGame->configStart();
	gameLogic->orderModify = unchanged;
}

void GameConfiguringScreen::update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);
	
	if (gameLogic->prevMenu.simple()) {
		gameLogic->transitionTo(&gameLogic->gameSelectScreen);
	}
	
	if (gameLogic->nextMenu.simple()) {
		if (gameLogic->playerContainer->getNumberOfPlayers() == 0) {
			gameLogic->transitionTo(&gameLogic->playerScreen);
		}
		else {
			gameLogic->transitionTo(&gameLogic->customGameConfiguringScreen);
		}
	}
	
	DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();
	currentGame->config(touch);
}

void GameConfiguringScreen::processMessage(JsonObject body) {
	DartsGame* currentGame = gameLogic->gameContainer->findGameByID(body["GAME"]);
	gameLogic->gameContainer->setCurrentGame(currentGame);
	
	JsonObject configObject = body["CONFIG"];
	currentGame->processConfig(configObject);
	gameLogic->transitionTo(this);
}
#include "GameConfiguringScreen.h"
#include "GameLogic.h"

void GameConfiguringScreen::start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(CYAN);
	
	int buttonSize = SCR_WIDTH / 10;
	int squareOffset = int(SCR_WIDTH * 0.08f);

    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(),  squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(),  SCR_WIDTH-squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">>", 2);
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);

	DartsGame* currentGame = DartsGame::getCurrentGame();

	String text = currentGame->getName() + " settings";
	int textSize = 2;
	int yHeight = int(SCR_HEIGHT * 0.1f);

	DisplayContainer::displayContainer.writeCenterX(yHeight, BLACK, CYAN, textSize, text);
    
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
		if (Player::getNumberOfPlayers() == 0) {
			gameLogic->transitionTo(&gameLogic->playerScreen);
		}
		else {
			gameLogic->transitionTo(&gameLogic->customGameConfiguringScreen);
		}
	}
	
	DartsGame* currentGame = DartsGame::getCurrentGame();
	currentGame->config(touch);
}

void GameConfiguringScreen::processMessage(JsonObject body) {
	DartsGame* currentGame = DartsGame::findGameByID(body["GAME"]);
	DartsGame::setCurrentGame(currentGame);
	
	JsonObject configObject = body["CONFIG"];
	currentGame->processConfig(configObject);
	gameLogic->transitionTo(this);
}
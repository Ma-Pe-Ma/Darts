#include "PostConfig.h"
#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

void PostConfig::start() {
    gamePlayingScreen->getGameLogic()->displayContainer->getTFT()->fillScreen(TFT_WHITE);
		
    String text = "Visszavago?";
    int y = int(SCR_HEIGHT * 0.0625);
    gamePlayingScreen->getGameLogic()->displayContainer->writeCenterX(y, TFT_BLACK, TFT_MAGENTA, 4, text.c_str());

	int buttonSize = int(SCR_WIDTH * 0.1f);

	int squareOffset = int(SCR_WIDTH * 0.08f);
    
    gameLogic->prevMenu.setImage(gamePlayingScreen->getGameLogic()->displayContainer->getTFT(), squareOffset, squareOffset, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(gamePlayingScreen->getGameLogic()->displayContainer->getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, TFT_WHITE, TFT_CYAN, TFT_BLACK, ">>", 2);
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);
    
	int optionStartX = SCR_WIDTH / 4;
	int optionStartY = SCR_HEIGHT / 2;

	int optionOffsetY = SCR_HEIGHT / 6;

	int optionWidth = SCR_WIDTH / 3;
	int optionHeight = SCR_HEIGHT / 6;

    gameLogic->prevCursor.setImage(gamePlayingScreen->getGameLogic()->displayContainer->getTFT(), optionStartX, optionStartY + 0 * optionOffsetY, optionWidth, optionHeight, TFT_GREEN, TFT_CYAN, TFT_BLACK, "Ciklikus", 2);
    gameLogic->middleCursor.setImage(gamePlayingScreen->getGameLogic()->displayContainer->getTFT(), optionStartX, optionStartY + 1 * optionOffsetY, optionWidth, optionHeight, TFT_GREEN, TFT_CYAN, TFT_BLACK, "Ford.", 2);
    gameLogic->nextCursor.setImage(gamePlayingScreen->getGameLogic()->displayContainer->getTFT(), optionStartX, optionStartY + 2 * optionOffsetY, optionWidth, optionHeight, TFT_GREEN, TFT_CYAN, TFT_BLACK, "UA.", 2);
    
    gameLogic->prevCursor.guiButton.drawButton(false);
    gameLogic->middleCursor.guiButton.drawButton(true);
    gameLogic->nextCursor.guiButton.drawButton(true);

	//default is cyclical!
	gameLogic->orderModify = cyclical;
}

void PostConfig::update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);
	
	if (gameLogic->prevMenu.simple()) {
		gameLogic->transitionTo(&gameLogic->gameConfiguringScreen);	
	}
	
	if (gameLogic->nextMenu.simple()) {
		gameLogic->transitionTo(&gameLogic->gamePlayingScreen);	
	}
	
	gameLogic->prevCursor.detect(touch);
	gameLogic->middleCursor.detect(touch);
	gameLogic->nextCursor.detect(touch);
	
	if (gameLogic->prevCursor.simple()) {
		gameLogic->prevCursor.guiButton.drawButton(false);
		gameLogic->middleCursor.guiButton.drawButton(true);
		gameLogic->nextCursor.guiButton.drawButton(true);
		gameLogic->orderModify = cyclical;
	}
	
	if (gameLogic->middleCursor.simple()) {
		gameLogic->prevCursor.guiButton.drawButton(true);
		gameLogic->middleCursor.guiButton.drawButton(false);
		gameLogic->nextCursor.guiButton.drawButton(true);
		gameLogic->orderModify = reversed;
	}
	
	if (gameLogic->nextCursor.simple()) {		
		gameLogic->prevCursor.guiButton.drawButton(true);
		gameLogic->middleCursor.guiButton.drawButton(true);
		gameLogic->nextCursor.guiButton.drawButton(false);
		gameLogic->orderModify = unchanged;
	}
}
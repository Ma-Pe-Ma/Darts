#include "PostConfig.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

void PostConfig::start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(WHITE);
		
    String text = "Visszavago?";
    int y = int(SCR_HEIGHT * 0.0625);
    DisplayContainer::displayContainer.writeCenterX(y, BLACK, MAGENTA, 4, text);

	int buttonSize = int(SCR_WIDTH * 0.1f);

	int squareOffset = int(SCR_WIDTH * 0.08f);
    
    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(), squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">>", 2);
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);
    
	int optionStartX = SCR_WIDTH / 4;
	int optionStartY = SCR_HEIGHT / 2;

	int optionOffsetY = SCR_HEIGHT / 6;

	int optionWidth = SCR_WIDTH / 3;
	int optionHeight = SCR_HEIGHT / 6;

    gameLogic->prevCursor.setImage(DisplayContainer::displayContainer.getTFT(), optionStartX, optionStartY + 0 * optionOffsetY, optionWidth, optionHeight, GREEN, CYAN, BLACK, "Ciklikus", 2);
    gameLogic->middleCursor.setImage(DisplayContainer::displayContainer.getTFT(), optionStartX, optionStartY + 1 * optionOffsetY, optionWidth, optionHeight, GREEN, CYAN, BLACK, "Ford.", 2);
    gameLogic->nextCursor.setImage(DisplayContainer::displayContainer.getTFT(), optionStartX, optionStartY + 2 * optionOffsetY, optionWidth, optionHeight, GREEN, CYAN, BLACK, "UA.", 2);
    
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
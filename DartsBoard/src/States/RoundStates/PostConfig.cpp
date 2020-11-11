#include "PostConfig.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

void PostConfig::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(WHITE);
		
    String text = "Visszavago?";
    int y = 15;
    DisplayContainer::displayContainer.WriteCenterX(y, BLACK, MAGENTA, 4, text);
    
    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(),  30, 30, 40, 40, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(),  370, 30, 40, 40, WHITE, CYAN, BLACK, ">>", 2);
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);
    
    gameLogic->prevCursor.setImage(DisplayContainer::displayContainer.getTFT(), 100, 120, 120, 40, GREEN, CYAN, BLACK, "Ciklikus", 2);
    gameLogic->middleCursor.setImage(DisplayContainer::displayContainer.getTFT(), 100, 160, 120, 40, GREEN, CYAN, BLACK, "Ford.", 2);
    gameLogic->nextCursor.setImage(DisplayContainer::displayContainer.getTFT(), 100, 200, 120, 40, GREEN, CYAN, BLACK, "UA.", 2);
    
    gameLogic->prevCursor.guiButton.drawButton(false);
    gameLogic->middleCursor.guiButton.drawButton(true);
    gameLogic->nextCursor.guiButton.drawButton(true);

	//default is cyclical!
	gameLogic->orderModify = cyclical;
}

void PostConfig::Update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);
	
	if (gameLogic->prevMenu.simple()) {
		gameLogic->TransitionTo(&gameLogic->gameConfiguringScreen);	
	}
	
	if (gameLogic->nextMenu.simple()) {
		gameLogic->TransitionTo(&gameLogic->gamePlayingScreen);	
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
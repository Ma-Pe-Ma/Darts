#include "GameSelectScreen.h"
#include "GameLogic.h"

void GameSelectScreen::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(GREEN);
		
    DisplayContainer::displayContainer.Write(25, 100, RED, 3, "Choose a gamescheme: ");
    DisplayContainer::displayContainer.WriteCenterX(150, RED, GREEN, 3, "               ");
    DisplayContainer::displayContainer.WriteCenterX(150, RED, GREEN, 3, DartsGame::dartsGame->name);
    gameLogic->prevCursor.setImage(DisplayContainer::displayContainer.getTFT(),  30, 160, 40, 40, WHITE, CYAN, BLACK, "<", 2);
    gameLogic->nextCursor.setImage(DisplayContainer::displayContainer.getTFT(), 370, 160, 40, 40, WHITE, CYAN, BLACK, ">", 2);
    
    gameLogic->prevCursor.guiButton.drawButton(true);
    gameLogic->nextCursor.guiButton.drawButton(true);
    
    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(),  30, 30, 40, 40, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(),  370, 30, 40, 40, WHITE, CYAN, BLACK, ">>", 2);
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);
    
    DartsGame::prevSelected = DartsGame::dartsGame;
}

void GameSelectScreen::Update(Pair touch) {
    gameLogic->prevCursor.detect(touch);
	gameLogic->nextCursor.detect(touch);
	
	gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);
	
    if (gameLogic->prevCursor.simple()) {
		gameLogic->prevCursor.guiButton.drawButton(false);
			
		DartsGame::dartsGame = DartsGame::prevGame();
		DisplayContainer::displayContainer.WriteCenterX(150, RED, GREEN, 3, "               ");
		DisplayContainer::displayContainer.WriteCenterX(150, RED, GREEN, 3, DartsGame::dartsGame->name);
	}
	
	if (gameLogic->prevCursor.released()) {
		gameLogic->prevCursor.guiButton.drawButton(true);
	}
	
	if (gameLogic->nextCursor.simple()) {
		gameLogic->nextCursor.guiButton.drawButton(false);
		
		DartsGame::dartsGame = DartsGame::nextGame();
		DisplayContainer::displayContainer.WriteCenterX(150, RED, GREEN, 3, "               ");
		DisplayContainer::displayContainer.WriteCenterX(150, RED, GREEN, 3, DartsGame::dartsGame->name);
	}
	
	if (gameLogic->nextCursor.released()) {
		gameLogic->nextCursor.guiButton.drawButton(true);
	}

	if (gameLogic->prevMenu.simple()) {
		gameLogic->TransitionTo(&gameLogic->playerScreen);
	}
	
	if (gameLogic->nextMenu.simple()) {		
		if (Player::number == 0) {
			gameLogic->TransitionTo(&gameLogic->playerScreen);
		}
		else {
			gameLogic->TransitionTo(&gameLogic->gameConfiguringScreen);
		}
	}
}

void GameSelectScreen::ProcessMessage(JsonObject message) {
    
}
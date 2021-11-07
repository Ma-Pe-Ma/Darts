#include "GameSelectScreen.h"
#include "GameLogic.h"

void GameSelectScreen::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(GREEN);
	
	int textSize = 3;
	int squareOffset = int(SCR_WIDTH * 0.08f);
	int buttonSize = int(SCR_WIDTH * 0.1f);

	int yPos = int(SCR_HEIGHT * 0.666f);

	//Write choosing + chosen game name
    DisplayContainer::displayContainer.Write(int(SCR_WIDTH * 0.0625f), int(SCR_HEIGHT * 0.40f), RED, textSize, "Choose a gamescheme: ");
    DisplayContainer::displayContainer.WriteCenterX(yPos - textSize * 6 / 2, RED, GREEN, textSize, "               ");
    DisplayContainer::displayContainer.WriteCenterX(yPos - textSize * 6 / 2, RED, GREEN, textSize, DartsGame::dartsGame->name);
    
	//draw game chooser buttons
	gameLogic->prevCursor.setImage(DisplayContainer::displayContainer.getTFT(), squareOffset, yPos, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<", 2);
    gameLogic->nextCursor.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH -squareOffset, yPos, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">", 2);
	
    gameLogic->prevCursor.guiButton.drawButton(true);
    gameLogic->nextCursor.guiButton.drawButton(true);
    
	//draw menu cursor buttons
    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(), squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">>", 2);
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);
    
    DartsGame::prevSelected = DartsGame::dartsGame;
}

void GameSelectScreen::Update(Pair touch) {
    //touch detection
	gameLogic->prevCursor.detect(touch);
	gameLogic->nextCursor.detect(touch);
	
	gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);
	
	//decrementing gametype
    if (gameLogic->prevCursor.simple()) {
		gameLogic->prevCursor.guiButton.drawButton(false);
		
		int textSize = 3;
		int yPos = int(SCR_HEIGHT * 0.666f);
		yPos -= textSize * 6 / 2;

		DartsGame::dartsGame = DartsGame::prevGame();
		DisplayContainer::displayContainer.WriteCenterX(yPos, RED, GREEN, textSize, "               ");
		DisplayContainer::displayContainer.WriteCenterX(yPos, RED, GREEN, textSize, DartsGame::dartsGame->name);
	}
	
	//incrementing gametype
	if (gameLogic->nextCursor.simple()) {
		gameLogic->nextCursor.guiButton.drawButton(false);

		int textSize = 3;
		int yPos = int(SCR_HEIGHT * 0.666f);
		yPos -= textSize * 6 / 2;
		
		DartsGame::dartsGame = DartsGame::nextGame();
		DisplayContainer::displayContainer.WriteCenterX(yPos, RED, GREEN, textSize, "               ");
		DisplayContainer::displayContainer.WriteCenterX(yPos, RED, GREEN, textSize, DartsGame::dartsGame->name);
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
		gameLogic->TransitionTo(&gameLogic->playerScreen);
	}
	
	//next to game config
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
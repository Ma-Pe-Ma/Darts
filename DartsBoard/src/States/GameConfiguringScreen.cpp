#include "GameConfiguringScreen.h"
#include "GameLogic.h"

void GameConfiguringScreen::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(CYAN);
    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(),  30, 30, 40, 40, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(),  370, 30, 40, 40, WHITE, CYAN, BLACK, ">>", 2);
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);

	String text = DartsGame::dartsGame->name + " settings";
	int size = 2;
	DisplayContainer::displayContainer.WriteCenterX(25, BLACK, CYAN, size, text);
    
    DartsGame::dartsGame->PreConfig();
	gameLogic->orderModify = unchanged;
}

void GameConfiguringScreen::Update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);
	
	if (gameLogic->prevMenu.simple()) {
		gameLogic->TransitionTo(&gameLogic->gameSelectScreen);
	}
	
	if (gameLogic->nextMenu.simple()) {
		if (Player::number == 0) {
			gameLogic->TransitionTo(&gameLogic->playerScreen);
		}
		else {
			gameLogic->TransitionTo(&gameLogic->customGameConfiguringScreen);
		}
	}
	
	DartsGame::dartsGame->GameConfig(touch);
}

void GameConfiguringScreen::ProcessMessage(JsonObject body) {
    DartsGame::dartsGame = DartsGame::findGameByID(body["GAME"]);
	JsonObject configObject = body["CONFIG"];
	DartsGame::dartsGame->ProcessConfig(configObject);
	gameLogic->TransitionTo(this);
}
#include "CustomGameConfiguringScreen.h"
#include "GameLogic.h"

void CustomGameConfiguringScreen::start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(YELLOW);
	
    DartsGame* currentGame = DartsGame::getCurrentGame();

    if (!currentGame->customStart()) {
        gameLogic->transitionTo(&gameLogic->gamePlayingScreen);
        gameLogic->notifyAboutStart();
        return;
    }

    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);    
}

void CustomGameConfiguringScreen::update(Pair touch) {
    DartsGame* currentGame = DartsGame::getCurrentGame();
    currentGame->custom(touch);
	
    gameLogic->prevMenu.detect(touch);
    gameLogic->nextMenu.detect(touch);

    if (gameLogic->prevMenu.simple()) {
        gameLogic->transitionTo(&gameLogic->gameConfiguringScreen);
    }

    if (gameLogic->nextMenu.simple()) {
        if (Player::getNumberOfPlayers() == 0) {
            gameLogic->transitionTo(&gameLogic->playerScreen);
        }
        else {
            gameLogic->transitionTo(&gameLogic->gamePlayingScreen);
            gameLogic->notifyAboutStart();
        }
    }
}

void CustomGameConfiguringScreen::processMessage(JsonObject message) {
    
}
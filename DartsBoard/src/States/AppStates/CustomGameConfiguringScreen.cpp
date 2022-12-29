#include "CustomGameConfiguringScreen.h"
#include "../GameLogic.h"

void CustomGameConfiguringScreen::start() {
    getGameLogic()->displayContainer->getTFT()->fillScreen(TFT_YELLOW);
	
    DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();

    if (!currentGame->customStart()) {
        gameLogic->transitionTo(&gameLogic->gamePlayingScreen);
        gameLogic->notifyAboutStart();
        return;
    }

    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);    
}

void CustomGameConfiguringScreen::update(Pair touch) {
    DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();
    currentGame->custom(touch);
	
    gameLogic->prevMenu.detect(touch);
    gameLogic->nextMenu.detect(touch);

    if (gameLogic->prevMenu.simple()) {
        gameLogic->transitionTo(&gameLogic->gameConfiguringScreen);
    }

    if (gameLogic->nextMenu.simple()) {
        if (gameLogic->playerContainer->getNumberOfPlayers() == 0) {
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
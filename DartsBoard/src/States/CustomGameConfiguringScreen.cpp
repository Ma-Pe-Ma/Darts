#include "CustomGameConfiguringScreen.h"
#include "GameLogic.h"

void CustomGameConfiguringScreen::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(YELLOW);
	
    if (!DartsGame::dartsGame->PreCustom()) {
        gameLogic->TransitionTo(&gameLogic->gamePlayingScreen);
        gameLogic->NotifyAboutStart();
        return;
    }

    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);    
}

void CustomGameConfiguringScreen::Update(Pair touch) {
    DartsGame::dartsGame -> CustomConfig(touch);
	
    gameLogic->prevMenu.detect(touch);
    gameLogic->nextMenu.detect(touch);

    if (gameLogic->prevMenu.simple()) {
        gameLogic->TransitionTo(&gameLogic->gameConfiguringScreen);
    }

    if (gameLogic->nextMenu.simple()) {
        if (Player::number == 0) {
            gameLogic->TransitionTo(&gameLogic->playerScreen);
        }
        else {
            gameLogic->TransitionTo(&gameLogic->gamePlayingScreen);
            gameLogic->NotifyAboutStart();
        }
    }
}

void CustomGameConfiguringScreen::ProcessMessage(JsonObject message) {
    
}
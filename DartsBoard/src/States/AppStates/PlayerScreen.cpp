#include "PlayerScreen.h"
#include "../GameLogic.h"

void PlayerScreen::start() {
    playersString = Resources::getTextByID(Resources::Text::players);
    android1String = Resources::getTextByID(Resources::Text::playersAndroid1);
    android2String = Resources::getTextByID(Resources::Text::playersAndroid2);
    playerNrString = Resources::getTextByID(Resources::Text::playerNr);

    getGameLogic()->displayContainer->getTFT()->fillScreen(MAGENTA);
    
    int squareOffset = int(SCR_WIDTH * 0.08f);
    int buttonSize = int(SCR_WIDTH * 0.1f);

    gameLogic->prevMenu.setImage(getGameLogic()->displayContainer->getTFT(), squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">>", 2);
    
    getGameLogic()->displayContainer->writeCenterX(buttonSize / 2, BLACK, MAGENTA, 3, playersString);
    
    if (gameLogic->androidMode) {
        if (gameLogic->playerContainer->getNumberOfPlayers() == 0) {
            int textSize = 2;
            int y = (SCR_HEIGHT - 6 * textSize) / 2;
            getGameLogic()->displayContainer->writeCenterX(y, BLACK, MAGENTA, textSize, android1String);
            
            y += SCR_HEIGHT / 6;
            getGameLogic()->displayContainer->writeCenterX(y, BLACK, MAGENTA, textSize, android2String);
        }
        else {
            int xOffset = int(SCR_WIDTH * 0.15f);
            int xDiff = int(SCR_WIDTH * 0.45f);

            int yOffset = int(SCR_HEIGHT * 0.25f);
            int yDiff = int(SCR_HEIGHT * 0.2f);

            for(int i = 0; i < gameLogic->playerContainer->getNumberOfPlayers(); i++) {
                Player* checkable = gameLogic->playerContainer->getPlayerByNumber(i);

                int x = xOffset + (i / 4) * xDiff; 
                int y = yOffset + (i % 4) * yDiff;
                String text = "P" + String(i + 1) + ":" + checkable->getNick();
                int textSize = 3;
                getGameLogic()->displayContainer->writeWithBackground(x, y, checkable->getInverseColor(), checkable->getColor(), textSize, text);
            }
            
            gameLogic->nextMenu.guiButton.drawButton(true);
        }
    }
    else {
        gameLogic->playerContainer->setNumberOfPlayers(1);

        //Writing explanation text
        int x = (SCR_WIDTH - playerNrString.length() * 6 * 2) / 2 - int(SCR_WIDTH * 0.1375f);
        int y = int(SCR_HEIGHT * 0.4f);
        getGameLogic()->displayContainer->writeWithBackground(x, y, BLACK, MAGENTA, 2, (char*) playerNrString.c_str());

        //drawing player count + buttons
        int yPos = int(SCR_HEIGHT * 0.666f);
        int textSize = 3;
        getGameLogic()->displayContainer->writeCenterX(yPos - textSize * 6 / 2, BLACK, MAGENTA, textSize, String(gameLogic->playerContainer->getNumberOfPlayers()));

        gameLogic->prevCursor.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH / 2 - int(buttonSize * 1.1f), yPos, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<", 2);
        gameLogic->nextCursor.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH / 2 + int(buttonSize * 1.1f), yPos, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">", 2);
    
        gameLogic->prevCursor.guiButton.drawButton(true);
        gameLogic->nextCursor.guiButton.drawButton(true);
        
        gameLogic->nextMenu.guiButton.drawButton(true);
    }
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->orderModify = unchanged;
}

void PlayerScreen::update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	
	if (gameLogic->prevMenu.simple()) {
		gameLogic->transitionTo(&gameLogic->mainScreen);
	}
	
	if (gameLogic->androidMode) {
		if (gameLogic->playerContainer->getNumberOfPlayers() != 0) {
			gameLogic->nextMenu.detect(touch);
			
			if (gameLogic->nextMenu.simple()) {
				gameLogic->transitionTo(&gameLogic->gameSelectScreen);
			}
		}		
	}
	else {
		gameLogic->nextMenu.detect(touch);
	
        //pushing nextmenu
		if (gameLogic->nextMenu.simple()) {
			gameLogic->transitionTo(&gameLogic->gameSelectScreen);
		}
		
		gameLogic->prevCursor.detect(touch);
		gameLogic->nextCursor.detect(touch);
		
        //pushing player decrement
		if (gameLogic->prevCursor.simple()) {
			gameLogic->prevCursor.guiButton.drawButton(false);
            gameLogic->playerContainer->setNumberOfPlayers(gameLogic->playerContainer->getNumberOfPlayers() - 1);
			if (gameLogic->playerContainer->getNumberOfPlayers() == 0) {
                gameLogic->playerContainer->setNumberOfPlayers(gameLogic->playerContainer->maxNrOfPlayers);
			}
			
            int textSize = 3;
            int yPos = int(SCR_HEIGHT * 0.666f);
            yPos -= textSize * 6 / 2;

			getGameLogic()->displayContainer->writeCenterX(yPos, BLACK, MAGENTA, textSize, String(gameLogic->playerContainer->getNumberOfPlayers()));
		}
		
        //pushing player increment
		if (gameLogic->nextCursor.simple()) {
			gameLogic->nextCursor.guiButton.drawButton(false);
            gameLogic->playerContainer->setNumberOfPlayers(gameLogic->playerContainer->getNumberOfPlayers() + 1);

			if (gameLogic->playerContainer->getNumberOfPlayers() == gameLogic->playerContainer->maxNrOfPlayers + 1) {
				gameLogic->playerContainer->setNumberOfPlayers(1);
			}

            int textSize = 3;
            int yPos = int(SCR_HEIGHT * 0.666f);
            yPos -= textSize * 6 / 2;
			
			getGameLogic()->displayContainer->writeCenterX(yPos, BLACK, MAGENTA, 3, String(gameLogic->playerContainer->getNumberOfPlayers()));
		}
		
        //visualizaton 
		if (gameLogic->prevCursor.released()) {
			gameLogic->prevCursor.guiButton.drawButton(true);
		}
		
		if (gameLogic->nextCursor.released()) {
			gameLogic->nextCursor.guiButton.drawButton(true);
		}
	}
}

void PlayerScreen::processMessage(JsonObject body) {
    JsonVariant players = body["PLAYERS"];
	gameLogic->parsePlayers(players);

    unsigned long gameNr = body["GAMENR"].as<unsigned long>();
    gameLogic->gamePlayingScreen.setGameNr(gameNr);

    gameLogic->transitionTo(this);
}
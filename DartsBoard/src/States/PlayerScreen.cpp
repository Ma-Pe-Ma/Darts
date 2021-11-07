#include "PlayerScreen.h"
#include "GameLogic.h"

void PlayerScreen::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(MAGENTA);
    
    int squareOffset = int(SCR_WIDTH * 0.08f);
    int buttonSize = int(SCR_WIDTH * 0.1f);

    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(), squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">>", 2);
    
    DisplayContainer::displayContainer.WriteCenterX(buttonSize / 2, BLACK, MAGENTA, 3, "JATEKOSOK");
    
    if (gameLogic->androidMode) {
        if (Player::number == 0) {
            String text = "Kerem allitsa be a jatekosokat";
            int textSize = 2;
            int y = (SCR_HEIGHT - 6 * textSize) / 2;
            DisplayContainer::displayContainer.WriteCenterX(y, BLACK, MAGENTA, textSize, text);
            
            text = "az Android kliensen!";
            y += SCR_HEIGHT / 6;
            DisplayContainer::displayContainer.WriteCenterX(y, BLACK, MAGENTA, textSize, text);
        }
        else {
            int xOffset = int(SCR_WIDTH * 0.15f);
            int xDiff = int(SCR_WIDTH * 0.45f);

            int yOffset = int(SCR_HEIGHT * 0.25f);
            int yDiff = int(SCR_HEIGHT * 0.2f);

            for(int i = 0; i < Player::number; i++) {
                int x = xOffset + (i / 4) * xDiff; 
                int y = yOffset + (i % 4) * yDiff;
                String text = "P" + String(i + 1) + ":" + Player::players[i].nickname;
                DisplayContainer::displayContainer.WriteWithBackground(x, y, Player::players[i].inverseColor, Player::players[i].color, 3, text);
            }
            
            gameLogic->nextMenu.guiButton.drawButton(true);
        }
    }
    else {
        Player::number = 1;

        //Writing explanation text
        String text = "Jatekosok szama: ";
        int x = (SCR_WIDTH - text.length() * 6 * 2) / 2 - int(SCR_WIDTH * 0.1375f);
        int y = int(SCR_HEIGHT * 0.4f);
        DisplayContainer::displayContainer.WriteWithBackground(x, y, BLACK, MAGENTA, 2, text);


        //drawing player count + buttons
        int yPos = int(SCR_HEIGHT * 0.666f);
        int textSize = 3;
        DisplayContainer::displayContainer.WriteCenterX(yPos - textSize * 6 / 2, BLACK, MAGENTA, textSize, String(Player::number));

        gameLogic->prevCursor.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH / 2 - int(buttonSize * 1.1f), yPos, buttonSize, buttonSize, WHITE, CYAN, BLACK, "<", 2);
        gameLogic->nextCursor.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH / 2 + int(buttonSize * 1.1f), yPos, buttonSize, buttonSize, WHITE, CYAN, BLACK, ">", 2);
    
        gameLogic->prevCursor.guiButton.drawButton(true);
        gameLogic->nextCursor.guiButton.drawButton(true);
        
        gameLogic->nextMenu.guiButton.drawButton(true);
    }
    
    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->orderModify = unchanged;
}

void PlayerScreen::Update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	
	if (gameLogic->prevMenu.simple()) {
		gameLogic->TransitionTo(&gameLogic->mainScreen);
	}
	
	if (gameLogic->androidMode) {
		if (Player::number != 0) {
			gameLogic->nextMenu.detect(touch);
			
			if (gameLogic->nextMenu.simple()) {
				gameLogic->TransitionTo(&gameLogic->gameSelectScreen);
			}
		}		
	}
	else {
		gameLogic->nextMenu.detect(touch);
	
        //pushing nextmenu
		if (gameLogic->nextMenu.simple()) {
			gameLogic->TransitionTo(&gameLogic->gameSelectScreen);
		}
		
		gameLogic->prevCursor.detect(touch);
		gameLogic->nextCursor.detect(touch);
		
        //pushing player decrement
		if (gameLogic->prevCursor.simple()) {
			gameLogic->prevCursor.guiButton.drawButton(false);
			if (--Player::number == 0) {
				Player::number = Player::maxNrOfPlayers;
			}
			
            int textSize = 3;
            int yPos = int(SCR_HEIGHT * 0.666f);
            yPos -= textSize * 6 / 2;

			DisplayContainer::displayContainer.WriteCenterX(yPos, BLACK, MAGENTA, textSize, String(Player::number));
		}
		
        //pushing player increment
		if (gameLogic->nextCursor.simple()) {
			gameLogic->nextCursor.guiButton.drawButton(false);
			if (++Player::number == Player::maxNrOfPlayers + 1) {
				Player::number = 1;
			}

            int textSize = 3;
            int yPos = int(SCR_HEIGHT * 0.666f);
            yPos -= textSize * 6 / 2;
			
			DisplayContainer::displayContainer.WriteCenterX(yPos, BLACK, MAGENTA, 3, String(Player::number));
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

void PlayerScreen::ProcessMessage(JsonObject body) {
    JsonVariant players = body["PLAYERS"];
	gameLogic->ParsePlayers(players);

    unsigned long gameNr = body["GAMENR"].as<unsigned long>();
    gameLogic->gamePlayingScreen.SetGameNr(gameNr);

    gameLogic->TransitionTo(this);
}
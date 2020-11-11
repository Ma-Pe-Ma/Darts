#include "PlayerScreen.h"
#include "GameLogic.h"

void PlayerScreen::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(MAGENTA);
		
    gameLogic->prevMenu.setImage(DisplayContainer::displayContainer.getTFT(),  30, 30, 40, 40, WHITE, CYAN, BLACK, "<<", 2);
    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(),  370, 30, 40, 40, WHITE, CYAN, BLACK, ">>", 2);
    
    DisplayContainer::displayContainer.WriteCenterX(20, BLACK, MAGENTA, 3, "JATEKOSOK");
    
    if (gameLogic->androidMode) {
        if (Player::number == 0) {
            String text = "Kerem allitsa be a jatekosokat";
            int textSize = 2;
            int y = (SCR_HEIGHT - 6 * textSize) / 2;
            DisplayContainer::displayContainer.WriteCenterX(y, BLACK, MAGENTA, textSize, text);
            
            text = "az Android kliensen!";
            y += 40;
            DisplayContainer::displayContainer.WriteCenterX(y, BLACK, MAGENTA, textSize, text);
        }
        else {
            for(int i = 0; i < Player::number; i++) {
                int x = 60 + (i / 4) * 180; 
                int y = 60 + (i % 4) * 50;
                String text = "P"+String(i+1)+":"+Player::players[i].nickname;
                DisplayContainer::displayContainer.WriteWithBackground(x, y, Player::players[i].inverseColor, Player::players[i].color, 3, text);
            }
            
            gameLogic->nextMenu.guiButton.drawButton(true);
        }
    }
    else {
        Player::number = 1;
        String text = "Jatekosok szama: ";
        int x = (400 - text.length() * 6 * 2) / 2 - 55;
        int y = 100;
        DisplayContainer::displayContainer.WriteWithBackground(x, y, BLACK, MAGENTA, 2, text);
        DisplayContainer::displayContainer.WriteCenterX(150, BLACK, MAGENTA, 3, String(Player::number));
        
        gameLogic->prevCursor.setImage(DisplayContainer::displayContainer.getTFT(), 155, 160, 40, 40, WHITE, CYAN, BLACK, "<", 2);
        gameLogic->nextCursor.setImage(DisplayContainer::displayContainer.getTFT(), 245, 160, 40, 40, WHITE, CYAN, BLACK, ">", 2);
    
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
	
		if (gameLogic->nextMenu.simple()) {
			gameLogic->TransitionTo(&gameLogic->gameSelectScreen);
		}
		
		gameLogic->prevCursor.detect(touch);
		gameLogic->nextCursor.detect(touch);
		
		if (gameLogic->prevCursor.simple()) {
			gameLogic->prevCursor.guiButton.drawButton(false);
			if (--Player::number == 0) {
				Player::number = Player::maxNrOfPlayers;
			}
			
			DisplayContainer::displayContainer.WriteCenterX(150, BLACK, MAGENTA, 3, String(Player::number));
		}
		
		if (gameLogic->nextCursor.simple()) {
			gameLogic->nextCursor.guiButton.drawButton(false);
			if (++Player::number == Player::maxNrOfPlayers + 1) {
				Player::number = 1;
			}
			
			DisplayContainer::displayContainer.WriteCenterX(150, BLACK, MAGENTA, 3, String(Player::number));
		}
		
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
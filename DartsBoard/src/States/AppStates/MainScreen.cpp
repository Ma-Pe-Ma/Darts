#include "MainScreen.h"
#include "../GameLogic.h"

//the height and width of the offline and android logo bitmap
#define ICON_SIZE 64

void MainScreen::start() {
    getGameLogic()->displayContainer->getTFT()->fillScreen(WHITE);

    //draw the main logo, supposing its height and width equals screen size
    getGameLogic()->displayContainer->showBMP("darts.bmp", (SCR_WIDTH - SCR_HEIGHT) / 2, 0);

    //show top texts
    int squareOffset = int(SCR_WIDTH * 0.04f);
    getGameLogic()->displayContainer->write(squareOffset, squareOffset, RED, 3, "MPM");
    getGameLogic()->displayContainer->writeRight(squareOffset, squareOffset, RED, 3, "Dartsi");

    //show icons at the bottom
    getGameLogic()->displayContainer->showBMP("offline.bmp", 0, SCR_HEIGHT - ICON_SIZE);
    getGameLogic()->displayContainer->showBMP("android.bmp", SCR_WIDTH - ICON_SIZE, SCR_HEIGHT - ICON_SIZE);
    
    //set up buttons for icons
    gameLogic->offline.setImage(getGameLogic()->displayContainer->getTFT(),  0, SCR_HEIGHT - ICON_SIZE, ICON_SIZE, ICON_SIZE, WHITE, CYAN, BLACK, "offline", 2);
    gameLogic->android.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH - ICON_SIZE, SCR_HEIGHT - ICON_SIZE, ICON_SIZE, ICON_SIZE, WHITE, CYAN, BLACK, "android", 2);
}

void MainScreen::update(Pair pair) {
    gameLogic->android.detect(pair);
	gameLogic->offline.detect(pair);

    //pushing android button
	if (gameLogic->android.simple()) {	
		gameLogic->androidMode = true;
		gameLogic->playerContainer->setNumberOfPlayers(0);
		gameLogic->askConfigDump();
        gameLogic->transitionTo(&gameLogic->playerScreen);
	}
	
    //pushing offline button
	if (gameLogic->offline.simple()) {
		gameLogic->androidMode = false;		
		gameLogic->playerContainer->setNumberOfPlayers(0);
        gameLogic->transitionTo(&gameLogic->playerScreen);
	}
}

void MainScreen::processMessage(JsonObject message) {
    
}
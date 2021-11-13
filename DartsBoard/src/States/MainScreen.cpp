#include "MainScreen.h"
#include "GameLogic.h"

//the height and width of the offline and android logo bitmap
#define ICON_SIZE 64

void MainScreen::start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(WHITE);

    //draw the main logo, supposing its height and width equals screen size
    ImageLoading::showBMP("darts.bmp", (SCR_WIDTH - SCR_HEIGHT) / 2, 0);

    //show top texts
    int squareOffset = int(SCR_WIDTH * 0.04f);
    DisplayContainer::displayContainer.write(squareOffset, squareOffset, RED, 3, "MPM");
    DisplayContainer::displayContainer.writeRight(squareOffset, squareOffset, RED, 3, "Dartsi");

    //show icons at the bottom
    ImageLoading::showBMP("offline.bmp", 0, SCR_HEIGHT - ICON_SIZE);
    ImageLoading::showBMP("android.bmp", SCR_WIDTH - ICON_SIZE, SCR_HEIGHT - ICON_SIZE);    
    
    //set up buttons for icons
    gameLogic->offline.setImage(DisplayContainer::displayContainer.getTFT(),  0, SCR_HEIGHT - ICON_SIZE, ICON_SIZE, ICON_SIZE, WHITE, CYAN, BLACK, "offline", 2);
    gameLogic->android.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH - ICON_SIZE, SCR_HEIGHT - ICON_SIZE, ICON_SIZE, ICON_SIZE, WHITE, CYAN, BLACK, "android", 2);
}

void MainScreen::update(Pair pair) {
    gameLogic->android.detect(pair);
	gameLogic->offline.detect(pair);

    //pushing android button
	if (gameLogic->android.simple()) {	
		gameLogic->androidMode = true;
		Player::setNumberOfPlayers(0);
		gameLogic->getConfigDump();
        gameLogic->transitionTo(&gameLogic->playerScreen);
	}
	
    //pushing offline button
	if (gameLogic->offline.simple()) {
		gameLogic->androidMode = false;		
		Player::setNumberOfPlayers(0);
        gameLogic->transitionTo(&gameLogic->playerScreen);
	}
}

void MainScreen::processMessage(JsonObject message) {
    
}
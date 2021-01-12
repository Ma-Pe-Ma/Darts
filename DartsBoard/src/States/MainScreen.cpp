#include "MainScreen.h"
#include "GameLogic.h"

void MainScreen::Start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(WHITE);
		
    DisplayContainer::displayContainer.getTFT()->setCursor(15, 15);
    DisplayContainer::displayContainer.getTFT()->setTextColor(RED, WHITE);
    DisplayContainer::displayContainer.getTFT()->setTextSize(3);
    DisplayContainer::displayContainer.getTFT()->print("MPM");

    showBMP("darts.bmp", 80, 0);
    DisplayContainer::displayContainer.WriteRight("Dartsi!", 3, 0, 15);
    showBMP("offline.bmp", 0, 176);
    showBMP("android.bmp", 336, 176);
    
    gameLogic->android.setImage(DisplayContainer::displayContainer.getTFT(),  336, 176, 64, 64, WHITE, CYAN, BLACK, "android", 2);
    gameLogic->offline.setImage(DisplayContainer::displayContainer.getTFT(),  0, 176, 64, 64, WHITE, CYAN, BLACK, "offline", 2);
}

void MainScreen::Update(Pair pair) {
    gameLogic->android.detect(pair);
	gameLogic->offline.detect(pair);

	if (gameLogic->android.simple()) {	
		gameLogic->androidMode = true;
		Player::number = 0;
		gameLogic->GetConfigDump();
        gameLogic->TransitionTo(&gameLogic->playerScreen);
	}
	
	if (gameLogic->offline.simple()) {
		gameLogic->androidMode = false;		
		Player::number = 0;

        gameLogic->TransitionTo(&gameLogic->playerScreen);
	}
}

void MainScreen::ProcessMessage(JsonObject message) {
    
}
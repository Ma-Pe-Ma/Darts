#include "Throwing.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

void Throwing::Start() {
	Player::current->score->Status();
}

void Throwing::Update(Pair pair) {
    if (gamePlayingScreen->GetGameLogic()->nextMenu.simple()) {
		gamePlayingScreen->GetGameLogic()->nextMenu.guiButton.drawButton(false);
		gamePlayingScreen->TransitionTo(&gamePlayingScreen->intro);
	}

	if (BoardContainer::boardContainer.dartsBoard->getKeys()) {
		// Scan the whole key list.
		for (int i = 0; i < LIST_MAX; i++) {
			//Only find keys that have changed state.
			if (BoardContainer::boardContainer.dartsBoard->key[i].stateChanged) {		
				// Report active key state : IDLE, PRESSED, HOLD, or RELEASED
				switch (BoardContainer::boardContainer.dartsBoard->key[i].kstate) {  
					case PRESSED:								
						BoardContainer::darts[BoardContainer::currentDart] = BoardContainer::SectorMapping(BoardContainer::boardContainer.dartsBoard->key[i].kchar);

						//if by scoring the player wins change to winning screen!
						if (Player::current->score->Score(BoardContainer::darts[BoardContainer::currentDart])) {
							gamePlayingScreen->TransitionTo(&gamePlayingScreen->winning);
						}
						//not winning show which sector was hit
						else {
							gamePlayingScreen->TransitionTo(&gamePlayingScreen->hit);
						}						
						break;
					case HOLD: {
						// msg = " HOLD.";
						gamePlayingScreen->stuckSector = BoardContainer::boardContainer.dartsBoard->key[i].kchar;
						Sector stuck = BoardContainer::SectorMapping(BoardContainer::boardContainer.dartsBoard->key[i].kchar);
						DisplayContainer::displayContainer.WriteWithBackground(SCR_WIDTH / 2, int(SCR_HEIGHT *0.04f), RED, CYAN, 2, "Stuck: " + DisplayContainer::SectorText(stuck));
						break;
					}
					case RELEASED:
						if (BoardContainer::boardContainer.dartsBoard->key[i].kchar == gamePlayingScreen->stuckSector) {
							DisplayContainer::displayContainer.WriteWithBackground(SCR_WIDTH / 2, int(SCR_HEIGHT *0.04f), RED, CYAN, 2, "          ");
						}
						// msg = " RELEASED.";
						break;
					case IDLE:
						;// msg = " IDLE.";
				}
			}
		}
	} 
}
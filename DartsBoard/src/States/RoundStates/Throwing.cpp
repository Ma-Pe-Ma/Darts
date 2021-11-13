#include "Throwing.h"
#include "../AppStates/GamePlayingScreen.h"
#include "../GameLogic.h"

#include "../../Scores/ThrowResult.h"

void Throwing::start() {
	gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer()->getScore()->status();
}

void Throwing::update(Pair pair) {
    if (gamePlayingScreen->getGameLogic()->nextMenu.simple()) {
		gamePlayingScreen->getGameLogic()->nextMenu.guiButton.drawButton(false);
		gamePlayingScreen->transitionTo(&gamePlayingScreen->intro);
	}

	Keypad* dartsBoard = gamePlayingScreen->boardContainer.getDartsBoard();

	if (dartsBoard->getKeys()) {
		// Scan the whole key list.
		for (int i = 0; i < LIST_MAX; i++) {
			//Only find keys that have changed state.
			if (dartsBoard->key[i].stateChanged) {		
				// Report active key state : IDLE, PRESSED, HOLD, or RELEASED

				ThrowResult throwResult;
				int currentID = gamePlayingScreen->boardContainer.getCurrentDartID();

				switch (dartsBoard->key[i].kstate) {  
					case PRESSED:
						gamePlayingScreen->boardContainer.setThrownDartByNumber(currentID, gamePlayingScreen->boardContainer.sectorMapping(dartsBoard->key[i].kchar));
						throwResult = gamePlayingScreen->getGameLogic()->playerContainer->getCurrentPlayer()->getScore()->scoreThrow(gamePlayingScreen->boardContainer.getThrownDartByNumber(currentID));

						switch (throwResult.throwType) {
							case ThrowType::normal:
								gamePlayingScreen->transitionTo(&gamePlayingScreen->hit);
								break;

							case ThrowType::busted:

								break;

							case ThrowType::winning:
								gamePlayingScreen->transitionTo(&gamePlayingScreen->winning);
								break;

							default:
								break;
						}

						break;
					case HOLD: {
						// msg = " HOLD.";
						gamePlayingScreen->stuckSector = dartsBoard->key[i].kchar;
						Sector stuck = gamePlayingScreen->boardContainer.sectorMapping(dartsBoard->key[i].kchar);
						gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(SCR_WIDTH / 2, int(SCR_HEIGHT * 0.04f), RED, CYAN, 2, "Stuck: " + DisplayContainer::sectorText(stuck));
						break;
					}
					case RELEASED:
						if (dartsBoard->key[i].kchar == gamePlayingScreen->stuckSector) {
							gamePlayingScreen->getGameLogic()->displayContainer->writeWithBackground(SCR_WIDTH / 2, int(SCR_HEIGHT * 0.04f), RED, CYAN, 2, "          ");
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
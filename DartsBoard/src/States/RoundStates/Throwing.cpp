#include "Throwing.h"
#include "../GamePlayingScreen.h"
#include "../GameLogic.h"

#include "../../Scores/ThrowResult.h"

void Throwing::start() {
	Player::getCurrentPlayer()->getScore()->status();
}

void Throwing::update(Pair pair) {
    if (gamePlayingScreen->getGameLogic()->nextMenu.simple()) {
		gamePlayingScreen->getGameLogic()->nextMenu.guiButton.drawButton(false);
		gamePlayingScreen->transitionTo(&gamePlayingScreen->intro);
	}

	if (BoardContainer::boardContainer.dartsBoard->getKeys()) {
		// Scan the whole key list.
		for (int i = 0; i < LIST_MAX; i++) {
			//Only find keys that have changed state.
			if (BoardContainer::boardContainer.dartsBoard->key[i].stateChanged) {		
				// Report active key state : IDLE, PRESSED, HOLD, or RELEASED

				ThrowResult throwResult;

				switch (BoardContainer::boardContainer.dartsBoard->key[i].kstate) {  
					case PRESSED:								
						BoardContainer::darts[BoardContainer::currentDart] = BoardContainer::sectorMapping(BoardContainer::boardContainer.dartsBoard->key[i].kchar);
						
						throwResult = Player::getCurrentPlayer()->getScore()->scoreThrow(BoardContainer::darts[BoardContainer::currentDart]);

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
						gamePlayingScreen->stuckSector = BoardContainer::boardContainer.dartsBoard->key[i].kchar;
						Sector stuck = BoardContainer::sectorMapping(BoardContainer::boardContainer.dartsBoard->key[i].kchar);
						DisplayContainer::displayContainer.writeWithBackground(SCR_WIDTH / 2, int(SCR_HEIGHT * 0.04f), RED, CYAN, 2, "Stuck: " + DisplayContainer::sectorText(stuck));
						break;
					}
					case RELEASED:
						if (BoardContainer::boardContainer.dartsBoard->key[i].kchar == gamePlayingScreen->stuckSector) {
							DisplayContainer::displayContainer.writeWithBackground(SCR_WIDTH / 2, int(SCR_HEIGHT * 0.04f), RED, CYAN, 2, "          ");
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
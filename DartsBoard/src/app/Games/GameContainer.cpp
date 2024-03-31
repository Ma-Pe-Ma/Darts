#include "GameContainer.h"

GameContainer::GameContainer(DisplayContainer* displayContainer, PlayerContainer* playerContainer) {
	this->displayContainer = displayContainer;
	this->playerContainer = playerContainer;
}

void GameContainer::init() {
	cricket.initialize(displayContainer, playerContainer);
	games[0] = &cricket;

	roundTheClock.initialize(displayContainer, playerContainer);
	games[1] = &roundTheClock;
	
	x01.initialize(displayContainer, playerContainer);
	games[2] = &x01;

	dartsGame = games[chosenGameCursor];
}

DartsGame* GameContainer::getGameByNr(int number) {
	return games[number];
}

DartsGame* GameContainer::getCurrentGame() {
	return this->dartsGame;
}

void GameContainer::setCurrentGame(DartsGame* dartsGame) {
	this->dartsGame = dartsGame;
}

DartsGame* GameContainer::findGameByID(String gameID) {	
	for (int i = 0; i < NUMBER_OF_GAMES; i++) {
		if (this->games[i]->getGameID() == gameID) {
			return this->games[i];
		}
	}
	
	return nullptr;
}

DartsGame* GameContainer::prevGame() {
	if (--chosenGameCursor < 0) {
		chosenGameCursor = NUMBER_OF_GAMES - 1;			
	}

	return games[chosenGameCursor];
}

DartsGame* GameContainer::nextGame() {
	if (++chosenGameCursor == NUMBER_OF_GAMES) {
		chosenGameCursor = 0;
	}

	return games[chosenGameCursor];
}
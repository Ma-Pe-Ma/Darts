#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#define NUMBER_OF_GAMES 3

#include "Cricket.h"
#include "RoundTheClock.h"
#include "X01.h"

class DisplayContainer;
class PlayerContainer;

class GameContainer {
    DartsGame* games[NUMBER_OF_GAMES];
	int chosenGameCursor;
	DartsGame* dartsGame;
	DisplayContainer* displayContainer;
	PlayerContainer* playerContainer;

	Cricket cricket;
	RoundTheClock roundTheClock;
	X01 x01;
public:
    GameContainer(DisplayContainer*, PlayerContainer*);
	void init();

    const int nrOfGames = NUMBER_OF_GAMES;
	void initializeGames();
	
	DartsGame* getGameByNr(int);	
	DartsGame* findGameByID(String);

	DartsGame* prevGame();
	DartsGame* nextGame();
	DartsGame* getCurrentGame();
	void setCurrentGame(DartsGame*);
};

#endif
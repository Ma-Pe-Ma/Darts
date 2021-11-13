#ifndef PLAYERCONTAINER_H
#define PLAYERCONTAINER_H

#include "Player.h"
#include "../Games/OrderModify.h"

#define NR_OF_PLAYERS 8

class PlayerContainer {
    int cursor;
	int number;
	Player* current;
	Player players[NR_OF_PLAYERS];
public:
    const int maxNrOfPlayers = NR_OF_PLAYERS;

    PlayerContainer();
	void init();

	Player* findNextPlayer();
	Player* getCurrentPlayer();
	void setCorrectPlayerOrder(OrderModify);
	
	Player* getPlayerByNumber(int);

	int getNumberOfPlayers();
	void setNumberOfPlayers(int);
	int getPlayerCursor();
	int getNumberOfFinishedPlayers();
};

#endif
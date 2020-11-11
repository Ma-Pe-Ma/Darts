#ifndef PLAYER_H
#define PLAYER_H

#define NR_OF_PLAYERS 8

#include "Hardware/Color.h"
#include "Scores/AbstractScore.h";

class Player {
private:
	
public:
	static int cursor;
	static int number;
	static Player* current;
	static const int maxNrOfPlayers;
	static Player players[NR_OF_PLAYERS];
	static void initializePlayers();
	static uint16_t convertColor(long);
	static uint16_t convertColor(uint8_t red, uint8_t green, uint8_t blue);
	static Player* FindNextPlayer();
	static void SetCorrectPlayerOrder();

	AbstractScore* score = nullptr;
	//Profile profile;
	int ID;
	String nickname = "";
	String name;
	uint16_t color;
	uint16_t inverseColor;	
};

#endif
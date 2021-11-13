#ifndef PLAYER_H
#define PLAYER_H

#define NR_OF_PLAYERS 8

#include "Hardware/Color.h"
#include "Scores/AbstractScore.h";

class Player {
private:
	static int cursor;
	static int number;
	static Player* current;
	static Player players[NR_OF_PLAYERS];

	int8_t ID;
	String nickname = "";
	String name;
	uint16_t color;
	uint16_t inverseColor;
	
	AbstractScore* score = nullptr;
public:
	static const int maxNrOfPlayers;
	static void initializePlayers();
	static uint16_t convertColor(long);
	static uint16_t convertColor(uint8_t red, uint8_t green, uint8_t blue);
	static Player* findNextPlayer();
	static Player* getCurrentPlayer();
	static void setCorrectPlayerOrder();
	
	static Player* getPlayerByNumber(int);

	static int getNumberOfPlayers();
	static void setNumberOfPlayers(int);
	static int getPlayerCursor();
	static int getNumberOfFinishedPlayers();

	uint16_t getInverseColor() {
		return this->inverseColor;
	}

	void setInverseColor(uint16_t inverseColor) {
		this->inverseColor = inverseColor;
	}

	uint16_t getColor() {
		return this->color;
	}

	void setColor(uint16_t color) {
		this->color = color;
	}

	int8_t getID() {
		return this->ID;
	}

	void setID(int8_t ID) {
		this->ID = ID;
	}

	AbstractScore* getScore() {
		return this->score;
	}

	void setScore(AbstractScore* score) {
		this->score = score;
	}

	String getNick() {
		return this->nickname;
	}

	void setNick(String nickname) {
		this->nickname = nickname;
	}

	String getName() {
		return this->name;
	}

	void setName(String name) {
		this->name = name;
	}
};

#endif
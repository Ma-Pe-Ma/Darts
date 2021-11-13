#ifndef DARTSGAME_H
#define DARTSGAME_H

#include "Arduino.h"
#include "../Hardware/Sector.h"
#include "../Player.h"
#include "../Hardware/BluetoothCommunicator.h"
#include "../Hardware/DisplayContainer.h"
#include "../Hardware/Pair.h"
#include "../Hardware/Button.h"
#include "KeyPad.h"
#include <ArduinoJson.h>

#define NUMBER_OF_GAMES 3

class DartsGame {
private:
	static DartsGame** games;
	static int chosenGameCursor;
	static DartsGame* dartsGame;
	static DartsGame* prevSelected;
protected:
	void serializeConfig();
	
	String gameID = "";
	String name = "";
	
public:
	virtual void serializeConfigCustom(JsonObject&) = 0;
	virtual void processConfig(JsonObject&) = 0;

	static BluetoothCommunicator* bluetoothCommunicator;	

	virtual void configStart() = 0;
	virtual void config(Pair) = 0;
		
	virtual bool customStart() = 0;
	virtual bool custom(Pair) = 0;
	
	virtual void initializeGame() = 0;
	virtual void initializeRound() = 0;
	virtual AbstractScore* setProperScoreContainer() = 0;
	
	static const int nrOfGames;
	static void initializeGames();
	
	static DartsGame* getGameByNr(int);
	
	static DartsGame* findGameByID(String);

	static DartsGame* prevGame();
	static DartsGame* nextGame();
	static DartsGame* getCurrentGame();
	static void setCurrentGame(DartsGame*);

	String getGameID() {
		return this->gameID;
	}

	void setGameID(String gameID) {
		this->gameID = gameID;
	}

	String getName() {
		return this->name;
	}

	void setName(String name) {
		this->name = name;
	}
};
#endif
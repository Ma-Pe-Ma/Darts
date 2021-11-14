#ifndef DARTSGAME_H
#define DARTSGAME_H

#include "Arduino.h"
#include "../Hardware/Sector.h"
#include "../Hardware/Bluetooth/BluetoothCommunicator.h"
#include "../Hardware/DisplayContainer.h"
#include "../Hardware/Pair.h"
#include "../Hardware/Button.h"
#include <ArduinoJson.h>

#include "../Scores/AbstractScore.h"

class PlayerContainer;

class DartsGame {
private:
	
protected:
	void serializeConfig();
	
	String gameID = "";
	String name = "";

	DisplayContainer* displayContainer;
	PlayerContainer* playerContainer;
	
public:
	DartsGame(DisplayContainer* displayContainer, PlayerContainer* playerContainer) {
		this->displayContainer = displayContainer;
		this->playerContainer = playerContainer;
	}

	virtual void serializeConfigCustom(JsonObject&) = 0;
	virtual void processConfig(JsonObject&) = 0;

	virtual void configStart() = 0;
	virtual void config(Pair) = 0;
		
	virtual bool customStart() = 0;
	virtual bool custom(Pair) = 0;
	
	virtual void initializeGame() = 0;
	virtual AbstractScore* setProperScoreContainer() = 0;

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
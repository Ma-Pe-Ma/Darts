#ifndef ABSTRACTSCORE_H
#define ABSTRACTSCORE_H

#include "../Hardware/DisplayContainer.h"
#include "../Hardware/Sector.h"
#include <ArduinoJson.h>
#include "ThrowResult.h"

class AbstractScore {
protected:
	int playerScore = 0;
	int position = -1;
	int roundCounter = 0;
public:
	virtual float getAverageScore() = 0;
	virtual void serializePlayerStatus(JsonObject&) = 0;
	virtual void serializeDartStatus(JsonObject body, Sector sector) = 0;

	virtual void drawCompleteCustomStatus() {}
	virtual ThrowResult scoreThrow(Sector) = 0;
	virtual void deleteThrow(Sector) = 0;
	virtual void status();
	virtual void correct(Sector&);
	virtual void statusAfterHit(Sector) {}

	int getPosition() {
		return this->position;
	}

	void setPosition(int position) {
		this->position = position;
	}

	int getPlayerScore() {
		return this->playerScore;
	}

	void setPlayerScore(int playerScore) {
		this->playerScore = playerScore;
	}

	int getRoundCounter() {
		return this->roundCounter;
	}

	void setRoundCounter(int roundCounter) {
		this->roundCounter = roundCounter;
	}
};

#endif
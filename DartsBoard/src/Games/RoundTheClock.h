#ifndef ROUNDTHECLOCK_H
#define ROUNDTHECLOCK_H

#include "DartsGame.h"
#include "../Scores/RTCScore.h"

class RoundTheClock : public DartsGame {	
private:

public:
	void configStart() override;
	void config(Pair) override;
	
	bool customStart() override;
	bool custom(Pair) override;
	
	void initializeGame() override;
	void initializeRound() override;
	
	RoundTheClock() {
		gameID = "RTC";
		name = "Round The Clock";
	}

	void serializeConfigCustom(JsonObject&) override;
	void processConfig(JsonObject&) override;

	AbstractScore* setProperScoreContainer() override {
		return new RTCScore();
	}
};
#endif
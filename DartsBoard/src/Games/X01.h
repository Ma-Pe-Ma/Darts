#ifndef X01_H
#define X01_H

#include "DartsGame.h"
#include "../Scores/X01Score.h"

class X01 : public DartsGame {
private:
	int startingValue;
	int busted;
	uint8_t out = 0, in = 0;
	void inOutString(int inValue, String& in, int outValue, String& out);

public:
	void configStart() override;
	void config(Pair) override;
	
	bool customStart() override;
	bool custom(Pair) override;
	
	void initializeGame() override;
	void initializeRound() override;

	void serializeConfigCustom(JsonObject&) override;
	void processConfig(JsonObject&) override;

	X01() {
		gameID = "X01";
		name = "X01";
	}

	AbstractScore* setProperScoreContainer() override {
		return new X01Score();
	}
};

#endif
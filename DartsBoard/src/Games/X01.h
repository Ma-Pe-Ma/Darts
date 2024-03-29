#ifndef X01_H
#define X01_H

#include "DartsGame.h"
#include "../Scores/X01Score.h"
#include "../SimpleMap.h"

#include "../Resource/Resources.h"

class X01 : public DartsGame {
private:
	int scoreCursor = 0;
	int inCursor = 0;
	int outCursor = 0;
	
	SimpleMap<int, int> scoreMap = SimpleMap<int, int>(5);

	Button previousScore, nextScore;
	Button inButton, outButton;

	char* simpleString;
	char* doubleString;
	char* masterString;

	char* inLabel;
	char* outLabel;
	char* scoreLabel;

	String inString;
	String outString;

	String getInOutString(int cursor);

	int buttonWidth = int(SCR_WIDTH * 0.35f);
	int buttonHeight = int(SCR_HEIGHT * 0.166f);	

	int buttonStartX = int(SCR_WIDTH * 0.25f);
	int buttonStartY = int(SCR_HEIGHT * 0.375f);
	
	int buttonOffsetX = int(SCR_WIDTH * 0.6f);
	int buttonOffsetY = int(SCR_HEIGHT * 0.25f);

public:
	void configStart() override;
	void config(Pair) override;
	
	bool customStart() override;
	bool custom(Pair) override;
	
	void initializeGame() override;
	
	void serializeConfigCustom(JsonObject&) override;
	void processConfig(JsonObject&) override;

	AbstractScore* setProperScoreContainer() override {
		X01Score* x01Score = new X01Score(displayContainer, playerContainer);
		x01Score->setGameProperties(inCursor, outCursor, scoreMap.getValueByKey(scoreCursor), (char*) inString.c_str(), (char*) outString.c_str());
		return x01Score;
	}

	void initialize(DisplayContainer*, PlayerContainer*);

	X01() {}
};

#endif
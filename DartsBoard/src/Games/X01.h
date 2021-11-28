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

	String simpleString;
	String doubleString;
	String masterString;

	String inLabel;
	String outLabel;
	String scoreLabel;

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

	X01(DisplayContainer* displayContainer, PlayerContainer* playerContainer) : DartsGame(displayContainer, playerContainer) {
		gameID = "X01";
		name = "X01";

		scoreMap.insert(0, 301);
		scoreMap.insert(1, 501);
		scoreMap.insert(2, 701);
		scoreMap.insert(3, 901);
		scoreMap.insert(4, 1001);

		simpleString = Resources::getTextByID(Resources::Text::x01Simple);
		doubleString = Resources::getTextByID(Resources::Text::x01Double);
		masterString = Resources::getTextByID(Resources::Text::x01Master);
		
		inLabel = Resources::getTextByID(Resources::Text::x01In);
		outLabel = Resources::getTextByID(Resources::Text::x01Out);
		scoreLabel = Resources::getTextByID(Resources::Text::x01Score);
	}

	AbstractScore* setProperScoreContainer() override {
		X01Score* x01Score = new X01Score(displayContainer, playerContainer);
		return x01Score;
	}
};

#endif
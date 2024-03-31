#ifndef ROUNDTHECLOCK_H
#define ROUNDTHECLOCK_H

#include "DartsGame.h"
#include "../Scores/RTCScore.h"

#include "../Resource/Resources.h"
#include "RTCEnums.h"

class RoundTheClock : public DartsGame {	
private:
	Button rtcButton, shootoutButton;
	Button sectorOnlyButton, multiplierButton, multiplierWithPointButton;

	int textSize = 2;
	int startButtonY = int(SCR_HEIGHT * 0.875f);
	int buttonSize = int(SCR_WIDTH * 0.1f);

	int textStartX = int(SCR_WIDTH * 0.6375f);
	int textStartY = startButtonY - buttonSize - int(textSize * 6);

	RTCType rtcType = roundTheClock;
	RTCSubType rtcSubType = sectorOnly;

	char* rtcClassicString;
	char* shootOutString;

	char* sectorOnlyString;
	char* multiplierString;
	char* multiplierWithPointsString;

	void drawProperButtons(int, int);
public:
	void configStart() override;
	void config(Pair) override;
	
	bool customStart() override;
	bool custom(Pair) override;
	
	void initializeGame() override;

	void serializeConfigCustom(JsonObject&) override;
	void processConfig(JsonObject&) override;

	AbstractScore* setProperScoreContainer() override {
		RTCScore* rtcScore = new RTCScore(displayContainer, playerContainer);
		rtcScore->setGameType(rtcType, rtcSubType);
		return rtcScore;
	}

	void initialize(DisplayContainer*, PlayerContainer*);

	RoundTheClock() {}
};
#endif
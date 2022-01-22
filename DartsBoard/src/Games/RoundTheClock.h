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

	String rtcClassicString;
	String shootOutString;

	String sectorOnlyString;
	String multiplierString;
	String multiplierWithPointsString;

	void drawProperButtons(int, int);
public:
	void configStart() override;
	void config(Pair) override;
	
	bool customStart() override;
	bool custom(Pair) override;
	
	void initializeGame() override;
	
	RoundTheClock(DisplayContainer* displayContainer, PlayerContainer* playerContainer) : DartsGame(displayContainer, playerContainer) {
		gameID = "RTC";
		name = "Round The Clock";

		sectorOnlyString = Resources::getTextByID(Resources::Text::rtcSector);
		multiplierString = Resources::getTextByID(Resources::Text::rtcMultiplier);
		multiplierWithPointsString = Resources::getTextByID(Resources::Text::rtcMultiplierWithPoints);

		rtcClassicString = Resources::getTextByID(Resources::Text::rtcClassic);
		shootOutString = Resources::getTextByID(Resources::Text::shootOut);
	}

	void serializeConfigCustom(JsonObject&) override;
	void processConfig(JsonObject&) override;

	AbstractScore* setProperScoreContainer() override {
		RTCScore* rtcScore = new RTCScore(displayContainer, playerContainer);
		rtcScore->setGameType(rtcType, rtcSubType);
		return rtcScore;
	}
};
#endif
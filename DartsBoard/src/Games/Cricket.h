#ifndef CRICKET_H
#define CRICKET_H

#include "DartsGame.h"
#include "../Scores/CricketScore.h"
#include "CricketEnums.h"

#include "../SimpleMap.h"

#include "../Resource/Resources.h"

class PlayerContainer;

class Cricket : public DartsGame {	
private:
	//buttons used by configuring
	Button scoreButton, noscoreButton, cutthroatButton;
	Button classicButton, allButton, customButton;
	Button intervalButton, randomIntervalButton, chaoticButton;
	
	void drawProperButtons(int, int);
	
	Button prevStart, nextStart;
	Button prevNr, nextNr;
	
	//drawing methods used by configuring screen
	void drawCricketStart();
	void drawCricketNr();

	//game type enums
	CricketType cricketType = score;
	CricketNumberSet cricketNumberSet = classicNumbers;
	CricketCustomSet cricketCustomSet = interval;

	void processConfig(ArduinoJson::JsonObject);
	void processGamePlay(ArduinoJson::JsonObject);

	uint8_t cricketNr = 6;
	uint8_t cricketStart = 15;
	const int defCricketMap[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

	int cricketMap[20] = {};

	SimpleMap<CricketType, String> typeMap;
	SimpleMap<CricketNumberSet, String> setMap;
	SimpleMap<CricketCustomSet, String> customMap;
	void initializeMaps();

	//used in custom game config
	int textSize = 2;
	int startButtonY = int(SCR_HEIGHT * 0.875f);
	int buttonSize = int(SCR_WIDTH * 0.1f);

	int textStartX = int(SCR_WIDTH * 0.6375f);
	int textStartY = startButtonY - buttonSize - int(textSize * 6);

	int rectSize = SCR_HEIGHT - textStartY;

	int scoreMap[21];

	String scoreString;
	String noscoreString;
	String cutthroatString;

	String classicString;
	String allString;
	String customString;

	String intervalString;
	String randintervalString;
	String chaoticString;

	String nrOfNrsString;
	String startingNrString;

public:
	void configStart() override;
	void config(Pair) override;
	
	bool customStart() override;
	bool custom(Pair) override;

	void initializeGame() override;

	void serializeConfigCustom(JsonObject& configObject);
	void processConfig(JsonObject&);

	AbstractScore* setProperScoreContainer() override {
		CricketScore* cricketScore = new CricketScore(displayContainer, playerContainer);
		cricketScore->setGameType(cricketType, cricketNumberSet, cricketCustomSet);
		cricketScore->setCricketMap(scoreMap);
		return cricketScore;
	}
	void initialize(DisplayContainer*, PlayerContainer*);

	Cricket() : typeMap(3), setMap(3), customMap(3) {}
};

#endif
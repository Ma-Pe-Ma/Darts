#ifndef CRICKETSCORE_H
#define CRICKETSCORE_H

#include "Arduino.h"
#include "AbstractScore.h"
#include "../Games/CricketEnums.h"

#include "../Hardware/Sector.h"
#include "../Player.h"

class CricketScore : public AbstractScore {
	int cricketStatus[21] = {};
	static int indicatorPositionMap[21];
	static CricketType cricketType;
	static CricketNumberSet cricketNumberSet;
	static CricketCustomSet cricketCustomSet;
	
public:
	static int scoreMap[21];

	static void createScoreMap(CricketNumberSet set, CricketCustomSet customSet, int cricketNr, int cricketStart);
	static int getIndicatorPosition(int);
	static int getMappedScore(Sector);
	static int getStatusPosition(Sector);
	static int getSectorCloseState(int);
	void serializeDartStatus(JsonObject body, Sector sector);
	
	void serializePlayerStatus(JsonObject&);

	void drawSectorIndicator(int, int, int, int);
	ThrowResult checkWinningCondition();
	CricketScore();
	
	void drawCompleteCustomStatus() override;
	ThrowResult scoreThrow(Sector) override;
	void deleteThrow(Sector) override;

	void statusAfterHit(Sector);

	float getAverageScore();
};

#endif
#ifndef CRICKETSCORE_H
#define CRICKETSCORE_H

#include "Arduino.h"
#include "AbstractScore.h"
#include "../Games/CricketEnums.h"

#include "../Hardware/Sector.h"

class CricketScore : public AbstractScore {
	int cricketStatus[21] = {};
	int scoreMap[21];

	void drawSectorIndicator(int, int, int, int);
	ThrowResult checkWinningCondition();

	CricketType cricketType;
	CricketNumberSet cricketNumberSet;
	CricketCustomSet cricketCustomSet;
	
	int getIndicatorPosition(Sector);
	int getMappedScore(Sector);
	int getStatusPosition(Sector);
	int getSectorCloseState(int);

public:
	CricketScore(DisplayContainer*, PlayerContainer*);
	
	float getAverageScore() override;
	ThrowResult scoreThrow(Sector) override;
	void deleteThrow(Sector) override;

	void drawCompleteCustomStatus() override;
	void statusAfterHit(Sector) override;

	void serializeDartStatus(JsonObject body, Sector sector) override;
	void serializePlayerStatus(JsonObject&) override;

	void setCricketMap(int*);
	void setGameType(CricketType, CricketNumberSet, CricketCustomSet);
};

#endif
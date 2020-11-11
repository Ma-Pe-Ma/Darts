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

	static void CreateScoreMap(CricketNumberSet set, CricketCustomSet customSet, int cricketNr, int cricketStart);
	static int GetIndicatorPosition(int);
	static int GetMappedScore(Sector);
	static int GetStatusPosition(Sector);
	static int GetSectorCloseState(int);
	void SerializeDartStatus(JsonObject body, Sector sector);
	
	void SerializePlayerStatus(JsonObject&);

	void DrawSectorIndicator(int, int, int, int);
	bool CheckWinningCondition();
	CricketScore();
	
	void DrawCompleteCustomStatus();
	bool Score(Sector);
	void Delete(Sector);

	void StatusAfterHit(Sector);

	float GetAverageScore();
};

#endif
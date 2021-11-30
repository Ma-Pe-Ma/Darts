#ifndef RTCSCORE_H
#define RTCSCORE_H

#include "AbstractScore.h"
#include "../Games/RTCEnums.h"

#define MULTI_MAX_SCORE 40

class RTCScore : public AbstractScore {
private:
    int currentNumber = 1;
    Sector nextNumber = {.base = 1, .multiplier = 0};

    RTCType rtcType;
    RTCSubType rtcSubType;
    void setNextNumber();

public:
    RTCScore(DisplayContainer*, PlayerContainer*);
    float getAverageScore() override;
    ThrowResult scoreThrow(Sector) override;
    void deleteThrow(Sector) override;

    void drawCompleteCustomStatus() override;
	void statusAfterHit(Sector) override;

    void serializePlayerStatus(JsonObject&) override;
    void serializeDartStatus(JsonObject body, Sector sector) override;

    void setGameType(RTCType, RTCSubType);

    void initThrowing() override;
};
#endif
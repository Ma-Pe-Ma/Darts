#ifndef RTCSCORE_H
#define RTCSCORE_H

#include "AbstractScore.h"

class RTCScore : public AbstractScore {

public:
    RTCScore(DisplayContainer*, PlayerContainer*);
    float getAverageScore() override;
    ThrowResult scoreThrow(Sector) override;
    void deleteThrow(Sector) override;

    void drawCompleteCustomStatus() override;
	void statusAfterHit(Sector) override;

    void serializePlayerStatus(JsonObject&) override;
    void serializeDartStatus(JsonObject body, Sector sector) override;
};
#endif
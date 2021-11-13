#ifndef RTCSCORE_H
#define RTCSCORE_H

#include "AbstractScore.h"

class RTCScore : public AbstractScore {

public:
    float getAverageScore() override;
    void serializePlayerStatus(JsonObject&) override;
    void serializeDartStatus(JsonObject body, Sector sector) override;

    void drawCompleteCustomStatus() override;
    ThrowResult scoreThrow(Sector) override;
    void deleteThrow(Sector) override;
};
#endif
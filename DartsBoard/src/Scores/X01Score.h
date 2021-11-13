#ifndef X01SCORE_H
#define X01SCORE_H

#include "AbstractScore.h"

class X01Score : public AbstractScore {

public:
    float getAverageScore() override;
    void serializePlayerStatus(JsonObject&) override;
    void serializeDartStatus(JsonObject body, Sector sector) override;

    void drawCompleteCustomStatus() override;
    ThrowResult scoreThrow(Sector) override;
    void deleteThrow(Sector) override;

};

#endif
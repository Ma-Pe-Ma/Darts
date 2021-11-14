#ifndef X01SCORE_H
#define X01SCORE_H

#include "AbstractScore.h"

class X01Score : public AbstractScore {

public:
    X01Score(DisplayContainer*, PlayerContainer*);
    
    float getAverageScore() override;
    ThrowResult scoreThrow(Sector) override;
    void deleteThrow(Sector) override;

    void drawCompleteCustomStatus() override;
	void statusAfterHit(Sector) override;

    void serializePlayerStatus(JsonObject&) override;
    void serializeDartStatus(JsonObject body, Sector sector) override;
};

#endif
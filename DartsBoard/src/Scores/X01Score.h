#ifndef X01SCORE_H
#define X01SCORE_H

#include "AbstractScore.h"

class X01Score : public AbstractScore {
private:
    uint8_t in = 0;
    uint8_t out = 0;
    int startingScore = 301;

    char* inString;
    char* outString;

public:
    X01Score(DisplayContainer*, PlayerContainer*);
    
    float getAverageScore() override;
    ThrowResult scoreThrow(Sector) override;
    void deleteThrow(Sector) override;

    void drawCompleteCustomStatus() override;
	void statusAfterHit(Sector) override;

    void serializePlayerStatus(JsonObject&) override;
    void serializeDartStatus(JsonObject body, Sector sector) override;

    void setGameProperties(uint8_t, uint8_t, int, char*, char*);
};

#endif
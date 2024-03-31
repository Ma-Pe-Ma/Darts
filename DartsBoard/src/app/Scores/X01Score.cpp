#include "X01Score.h"

X01Score::X01Score(DisplayContainer* displayContainer, PlayerContainer* playerContainer) : AbstractScore(displayContainer, playerContainer) {
    
}

float X01Score::getAverageScore() {
    return 0;
}

ThrowResult X01Score::scoreThrow(Sector sector) {
    ThrowResult throwResult;
    throwResult.throwType = ThrowType::normal;

    if (playerScore == startingScore) {
        switch (in) {
        case 1:
            if (sector.multiplier == 2) {
                throwResult.throwType = ThrowType::normal;
                playerScore -= sector.base * sector.multiplier;
            }
            else {
                throwResult.throwType = ThrowType::missed;
            }

            break;

        case 2:
            if (sector.multiplier > 1) {
                throwResult.throwType = ThrowType::normal;
                playerScore -= sector.base * sector.multiplier;
            }
            else {
                throwResult.throwType = ThrowType::missed;
            }

            break;

        default:
            throwResult.throwType = ThrowType::normal;
            playerScore -= sector.base * sector.multiplier;
            break;
        }
    }
    else {
        int newScore = playerScore - sector.base * sector.multiplier;
        if (newScore > 1) {
            playerScore = newScore;
            throwResult.throwType = ThrowType::normal;            
        }
        else if (newScore == 1) {
            if (out == 0) {
                playerScore = newScore;
                throwResult.throwType = ThrowType::normal;
            }
            else {
                throwResult.throwType = ThrowType::busted;
            }            
        }
        else if (newScore == 0) {
            switch (out) {
            case 1:
                if (sector.multiplier == 2) {
                    playerScore = newScore;
                    throwResult.throwType = ThrowType::winning;
                }
                else {
                    throwResult.throwType = ThrowType::busted;
                }
                break;

            case 2:
                if (sector.multiplier > 1) {
                    playerScore = newScore;
                    throwResult.throwType = ThrowType::winning;
                }
                else {
                    throwResult.throwType = ThrowType::busted;
                }
                break;

            default:
                playerScore = newScore;
                throwResult.throwType = ThrowType::winning;
                break;
            }
        }
        else if (newScore < 0) {
            throwResult.throwType = ThrowType::busted;
        }
    }

    return throwResult;
}

void X01Score::deleteThrow(Sector) {

}

void X01Score::drawCompleteCustomStatus() {
    int textSize = 2;

    int x = int(SCR_WIDTH * 0.1f);
    int y = int(SCR_HEIGHT * 0.6f) + (int(SCR_HEIGHT * 0.4f) - textSize * 6) / 2;

    displayContainer->write(x, y, TFT_BLACK, textSize, inString);
    displayContainer->write(SCR_WIDTH - x - strlen(outString) * textSize * 6, y, TFT_BLACK, textSize, outString);
}

void X01Score::statusAfterHit(Sector) {

}

void X01Score::setGameProperties(uint8_t in, uint8_t out, int startingScore, char* inString, char* outString) {
    this->in = in;
    this->out = out;
    this->startingScore = startingScore;
    this->inString = inString;
    this->outString = outString;

    playerScore = startingScore;
}

void X01Score::serializePlayerStatus(JsonObject&) {

}
void X01Score::serializeDartStatus(JsonObject body, Sector sector) {

}
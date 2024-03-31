#include "RTCScore.h"

RTCScore::RTCScore(DisplayContainer* displayContainer, PlayerContainer* playerContainer) : AbstractScore(displayContainer, playerContainer) {
    
}

float RTCScore::getAverageScore() {
    return 0;
}

void RTCScore::initThrowing() {
    switch (rtcType) {
        case roundTheClock:
            nextNumber.base = currentNumber;
            break;
        case shootOut:
            nextNumber.base = random(1, 22);
            break;
    }

    if (nextNumber.base == 21) {
        nextNumber.base = 25;
    }

    switch(rtcSubType) {
        case sectorOnly:
            nextNumber.multiplier = 4;
            break;
        case multiplier:
            nextNumber.multiplier = random(1,4);
            break;
        case multiplierWithPoint:
            nextNumber.multiplier = 4;
            break;
    }

    String sectorText = displayContainer->sectorText(nextNumber);
    int textSize = 6;

    if (rtcSubType != multiplierWithPoint) {
        int y = int(SCR_HEIGHT * 0.6f) + (int(SCR_HEIGHT * 0.4f) - textSize * 6) / 2;

        displayContainer->writeCenterX(y, TFT_YELLOW, TFT_BLUE, textSize, sectorText.c_str());
    }
    else {
        String scoreText = String(playerScore)  + "/" + String(MULTI_MAX_SCORE);
        displayContainer->writeWithBackground(int(0.1f * SCR_WIDTH), int(SCR_HEIGHT * 0.6f), TFT_YELLOW, TFT_BLUE, textSize, sectorText.c_str());
        displayContainer->writeWithBackground(int(0.5f * SCR_WIDTH), int(SCR_HEIGHT * 0.6f), TFT_YELLOW, TFT_BLUE, textSize, scoreText.c_str());
    }
}

void RTCScore::setNextNumber() {
    //if rtc determine next dart
    switch (rtcType) {
        case roundTheClock:
            currentNumber++;
            if (currentNumber == 22) {
                currentNumber = 1;
            }
            break;
        case shootOut:
            
            break;
    }  
}

ThrowResult RTCScore::scoreThrow(Sector sector) {
    ThrowResult throwResult;
    throwResult.throwType = ThrowType::normal;

    //calculate score
    switch (rtcSubType) {
        case sectorOnly:
            if (sector.base == nextNumber.base) {
                playerScore++;
                setNextNumber();
            }
            else {
                throwResult.throwType = ThrowType::missed;
            }
            break;
        case multiplier:
            if (sector.base == nextNumber.base && sector.multiplier == nextNumber.multiplier) {
                playerScore++;
                setNextNumber();
            }
            else {
                throwResult.throwType = ThrowType::missed;
            }
            break;

        case multiplierWithPoint:
            if (sector.base == nextNumber.base) {
                playerScore += nextNumber.multiplier;
                setNextNumber();
            }
            else {
                throwResult.throwType = ThrowType::missed;
            }
        default:
            break;
    }

    //check finishing condition
    switch (rtcSubType) {
        case sectorOnly:
            if (playerScore == 22) {
                throwResult.throwType = ThrowType::winning;
            }
            break;
        case multiplier:
            if (playerScore == 22) {
                throwResult.throwType = ThrowType::winning;
            }
            break;
        case multiplierWithPoint:
            if (playerScore >= MULTI_MAX_SCORE) {
                throwResult.throwType = ThrowType::winning;
            }
            break;
        default:
            break;
    }

    return throwResult;
}

void RTCScore::deleteThrow(Sector) {
    
}

void RTCScore::drawCompleteCustomStatus() {

}

void RTCScore::statusAfterHit(Sector) {

}

void RTCScore::setGameType(RTCType rtcType, RTCSubType rtcSubType) {
    this->rtcType = rtcType;
    this->rtcSubType = rtcSubType;

    currentNumber = 1;
}

void RTCScore::serializePlayerStatus(JsonObject&) {

}

void RTCScore::serializeDartStatus(JsonObject body, Sector sector) {

}
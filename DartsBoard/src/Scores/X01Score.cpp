#include "X01Score.h"

X01Score::X01Score(DisplayContainer* displayContainer, PlayerContainer* playerContainer) : AbstractScore(displayContainer, playerContainer) {
    
}

float X01Score::getAverageScore() {
    return 0;
}

ThrowResult X01Score::scoreThrow(Sector) {

}

void X01Score::deleteThrow(Sector) {

}

void X01Score::serializePlayerStatus(JsonObject&) {

}
void X01Score::serializeDartStatus(JsonObject body, Sector sector) {

}

void X01Score::drawCompleteCustomStatus() {

}

void X01Score::statusAfterHit(Sector) {

}
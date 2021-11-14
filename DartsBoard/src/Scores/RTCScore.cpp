#include "RTCScore.h"

RTCScore::RTCScore(DisplayContainer* displayContainer, PlayerContainer* playerContainer) : AbstractScore(displayContainer, playerContainer) {
    
}

float RTCScore::getAverageScore() {
    return 0;
}

ThrowResult RTCScore::scoreThrow(Sector) {

}

void RTCScore::deleteThrow(Sector) {
    
}

void RTCScore::serializePlayerStatus(JsonObject&) {

}
void RTCScore::serializeDartStatus(JsonObject body, Sector sector) {

}

void RTCScore::drawCompleteCustomStatus() {

}

void RTCScore::statusAfterHit(Sector) {

}

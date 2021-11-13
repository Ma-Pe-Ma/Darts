#include "LastState.h"
#include "GameLogic.h"

void LastState::start() {

}

void LastState::update(Pair pair) {
    
}

void LastState::processMessage(JsonObject message) {
    gameLogic->bluetoothCommunicator->repeatLastMessage();
}
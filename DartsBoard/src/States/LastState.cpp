#include "LastState.h"
#include "GameLogic.h"

void LastState::Start() {

}

void LastState::Update(Pair pair) {
    
}

void LastState::ProcessMessage(JsonObject message) {
    gameLogic->bluetoothCommunicator->repeatLastMessage();
}
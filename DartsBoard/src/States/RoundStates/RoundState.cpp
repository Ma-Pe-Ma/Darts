#include "RoundState.h"
#include "../GamePlayingScreen.h"

RoundState::RoundState(GamePlayingScreen* gamePlayingScreen) {
    this->gamePlayingScreen = gamePlayingScreen;
    this->gameLogic = gamePlayingScreen->getGameLogic();
}
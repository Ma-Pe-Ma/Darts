#include "RoundState.h"
#include "AppStates/GamePlayingScreen.h"

RoundState::RoundState(GamePlayingScreen* gamePlayingScreen) {
    this->gamePlayingScreen = gamePlayingScreen;
    this->gameLogic = gamePlayingScreen->getGameLogic();
}
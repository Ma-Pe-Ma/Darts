#ifndef ROUNDSTATE2
#define ROUNDSTATE2

#include "State.h"
#include "../Hardware/BoardContainer.h"
#include "../Hardware/DisplayContainer.h"

class GamePlayingScreen;
class GameLogic;

class RoundState : public State {
    
protected:
    unsigned long timer;
    GamePlayingScreen* gamePlayingScreen;
    GameLogic* gameLogic;
    RoundState(GamePlayingScreen* gamePlayingScreen);
};

#endif
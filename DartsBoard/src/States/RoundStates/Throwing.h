#ifndef THROWING_H
#define THROWING_H

#include "RoundState.h"

class Throwing : public RoundState {
public:
    Throwing(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
    void Start();
    void Update(Pair);
};

#endif
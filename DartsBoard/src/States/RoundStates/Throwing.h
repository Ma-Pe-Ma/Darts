#ifndef THROWING_H
#define THROWING_H

#include "RoundState.h"

class Throwing : public RoundState {
public:
    void start() override;
    void update(Pair) override;

    Throwing(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
};

#endif
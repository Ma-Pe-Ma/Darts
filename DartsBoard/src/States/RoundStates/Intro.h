#ifndef INTRO_H
#define INTRO_H

#include "RoundState.h"
#include "../../Games/DartsGame.h"

class Intro : public RoundState {
public:
    void start() override;
    void update(Pair) override;

    Intro(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
};

#endif
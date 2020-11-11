#ifndef INTRO_H
#define INTRO_H

#include "RoundState.h"
#include "../../Games/DartsGame.h"

class Intro : public RoundState {


public:
    Intro(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}

    void Start();
    void Update(Pair);

    bool firstTime = true;
};

#endif
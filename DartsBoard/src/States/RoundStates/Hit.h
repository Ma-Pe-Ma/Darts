#ifndef HIT_H
#define HIT_H

#include "RoundState.h"

class Hit : public RoundState {
    int dartStatusStartX = SCR_WIDTH / 10;
    int dartStatusOffsetX = SCR_WIDTH / 3;

    int dartStatusStartY = SCR_HEIGHT / 2;

public:
    Hit(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
    void Start();
    void Update(Pair);
};

#endif
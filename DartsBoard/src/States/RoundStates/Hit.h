#ifndef HIT_H
#define HIT_H

#include "RoundState.h"

class Hit : public RoundState {
public:
    Hit(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
    void Start();
    void Update(Pair);
};

#endif
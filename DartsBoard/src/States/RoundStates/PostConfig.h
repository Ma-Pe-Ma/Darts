#ifndef POSTCONFIGSCREEN_H
#define POSTCONFIGSCREEN_H

#include "RoundState.h"

class PostConfig : public RoundState {

public:
    void Start();
    void Update(Pair);
    PostConfig(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}

    void SendPostConfig(int);
};

#endif
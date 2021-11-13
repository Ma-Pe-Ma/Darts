#ifndef POSTCONFIGSCREEN_H
#define POSTCONFIGSCREEN_H

#include "RoundState.h"

class PostConfig : public RoundState {
public:
    void start() override;
    void update(Pair) override;  

    PostConfig(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}

    void SendPostConfig(int);
};

#endif
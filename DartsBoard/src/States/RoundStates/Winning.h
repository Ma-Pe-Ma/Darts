#ifndef WINNING_H
#define WINNING_H

#include "../RoundState.h"

class Winning : public RoundState {
public:
    void start();
    void update(Pair);

    Winning(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
    
    int winningTime = 5;
};

#endif
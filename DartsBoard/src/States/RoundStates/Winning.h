#ifndef WINNING_H
#define WINNING_H

#include "RoundState.h"

class Winning : public RoundState {
public:
    Winning(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
    void Start();
    void Update(Pair);
    
    int winningTime = 5;
};

#endif
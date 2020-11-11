#ifndef OUTRO_H
#define OUTRO_H

#include "RoundState.h"

class Outro : public RoundState {
    int s = 7;
	
	int x = 50;
	int y = 50;

	String text;

public:
    Outro(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
    void Start();
    void Update(Pair);
};

#endif
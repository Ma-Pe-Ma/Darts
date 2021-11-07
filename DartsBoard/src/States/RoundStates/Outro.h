#ifndef OUTRO_H
#define OUTRO_H

#include "RoundState.h"

class Outro : public RoundState {
    int textSize = 7;
	
	const int x = SCR_WIDTH / 8;
	int y = SCR_WIDTH / 8;

	String text;
public:
    Outro(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}
    void Start();
    void Update(Pair);
};

#endif
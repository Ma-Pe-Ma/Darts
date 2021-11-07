#ifndef GAMEPLAYINGSCREEN_H
#define GAMEPLAYINGSCREEN_H

#include "AppState.h"
#include "StateContext.h"
#include "../Player.h"

#include "RoundStates/Intro.h"
#include "RoundStates/Throwing.h"
#include "RoundStates/Hit.h"
#include "RoundStates/Outro.h"
#include "RoundStates/Winning.h"
#include "RoundStates/PostConfig.h"

class GamePlayingScreen : public AppState, public StateContext {
    void InvertDart(int);
    void Correct();

    void onDump(JsonObject&);
    void onNext(JsonObject&);
    void onCorrect(JsonObject&);
    void onDelete(JsonObject&);

    //thrown dart status properties
    int dartStatusStartX = 0;
	int dartStatusOffsetX = int(SCR_WIDTH * 0.333f);

	int dartStatusStartY = int(SCR_HEIGHT * 0.666f);

	int dartStatusWidth = int(SCR_WIDTH * 0.333f);
	int dartStatusHeight = SCR_HEIGHT - dartStatusStartY;

public:
    void Start();
    void Update(Pair);
    void CreatePlayerDump(JsonObject& playerBody, Player* player);
    GamePlayingScreen(GameLogic* gameLogic);
    void ProcessMessage(JsonObject);
    void SetGameNr(unsigned long gameNr) {
        this->gameNr = gameNr;
    }

    bool del[3];
    int roundCounter = 0;

    void SendStart();
    void SendHit(Sector);
    void SendDump();
	void SendRoundDump();

    int nextWinningPosition = 1;
    bool gameFinished = false;
    unsigned long gameNr = 0;
    char stuckSector = '\0';

    Intro intro = Intro(this);
    Throwing throwing = Throwing(this);
    Hit hit = Hit(this);
    Outro outro = Outro(this);
    Winning winning = Winning(this);
    PostConfig postConfig = PostConfig(this);
};

#endif
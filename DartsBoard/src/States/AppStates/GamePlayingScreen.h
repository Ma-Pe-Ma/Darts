#ifndef GAMEPLAYINGSCREEN_H
#define GAMEPLAYINGSCREEN_H

#include "../AppState.h"
#include "../StateContext.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerContainer.h"

#include "../../Hardware/BoardContainer.h"

#include "../RoundStates/Intro.h"
#include "../RoundStates/Throwing.h"
#include "../RoundStates/Hit.h"
#include "../RoundStates/Outro.h"
#include "../RoundStates/Winning.h"
#include "../RoundStates/PostConfig.h"

class GamePlayingScreen : public AppState, public StateContext {
    void invertDart(int);
    void correct();

    //client message processors
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
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;

    void createPlayerDump(JsonObject& playerBody, Player* player);
    GamePlayingScreen(GameLogic* gameLogic);
    
    void setGameNr(unsigned long gameNr) {
        this->gameNr = gameNr;
    }

    bool del[3];
    int roundCounter = 0;

    void sendStart();
    void sendHit(Sector);
    void sendDump();
	void sendRoundDump();

    unsigned long gameNr = 0;
    char stuckSector = '\0';

    //roundstates
    Intro intro = Intro(this);
    Throwing throwing = Throwing(this);
    Hit hit = Hit(this);
    Outro outro = Outro(this);
    Winning winning = Winning(this);
    PostConfig postConfig = PostConfig(this);

    BoardContainer boardContainer;
};

#endif
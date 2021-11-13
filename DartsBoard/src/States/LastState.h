#ifndef LASTSTATE_H
#define LASTSTATE_H

#include "AppState.h"

class LastState : public AppState {
public:
    void start();
    void update(Pair);
    void processMessage(JsonObject);

    LastState(GameLogic* gameLogic) : AppState(gameLogic) {ID = "LAST";}
};

#endif
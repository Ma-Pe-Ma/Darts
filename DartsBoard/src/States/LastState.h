#ifndef LASTSTATE_H
#define LASTSTATE_H

#include "AppState.h"

class LastState : public AppState {
    public:
    void Start();
    void Update(Pair);
    LastState(GameLogic* gameLogic) : AppState(gameLogic) {ID = "LAST";}
    void ProcessMessage(JsonObject);
};

#endif
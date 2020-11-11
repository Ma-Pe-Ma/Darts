#ifndef PLAYERSCREEN_H
#define PLAYERSCREEN_H

#include "AppState.h"

class PlayerScreen : public AppState {
public:
    void Start();
    void Update(Pair);
    PlayerScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "PLAYERS";}
    void ProcessMessage(JsonObject);
};

#endif
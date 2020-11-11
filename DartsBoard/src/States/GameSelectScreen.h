#ifndef GAMESELECTSCREEN_H
#define GAMESELECTSCREEN_H

#include "AppState.h"

class GameSelectScreen : public AppState {

public:
    void Start();
    void Update(Pair);
    GameSelectScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "SELECT";}
    void ProcessMessage(JsonObject);
};

#endif
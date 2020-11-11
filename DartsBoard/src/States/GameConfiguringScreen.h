#ifndef GAMECONFIGURINGSCREEN_H
#define GAMECONFIGURINGSCREEN_H

#include "AppState.h"

class GameConfiguringScreen : public AppState {

public:
    void Start();
    void Update(Pair);
    GameConfiguringScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "CONFIG";}
    void ProcessMessage(JsonObject);
};

#endif
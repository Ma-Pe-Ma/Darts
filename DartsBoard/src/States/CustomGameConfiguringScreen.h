#ifndef CUSTOMGAMECONFIGURINGSCREEN_H
#define CUSTOMGAMECONFIGURINGSCREEN_H

#include "AppState.h"

class CustomGameConfiguringScreen : public AppState {

public:
    void Start();
    void Update(Pair);
    CustomGameConfiguringScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "CUSTOM";}
    void ProcessMessage(JsonObject);
};

#endif
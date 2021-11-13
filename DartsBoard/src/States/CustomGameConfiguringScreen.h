#ifndef CUSTOMGAMECONFIGURINGSCREEN_H
#define CUSTOMGAMECONFIGURINGSCREEN_H

#include "AppState.h"

class CustomGameConfiguringScreen : public AppState {

public:
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;
    
    CustomGameConfiguringScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "CUSTOM";}    
};

#endif
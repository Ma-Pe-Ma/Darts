#ifndef GAMECONFIGURINGSCREEN_H
#define GAMECONFIGURINGSCREEN_H

#include "../AppState.h"
#include "../../Resource/Resources.h"

class GameConfiguringScreen : public AppState {
protected:
    char* settingsString;
public:
    void initialize() override;
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;

    GameConfiguringScreen(GameLogic* gameLogic) : AppState(gameLogic) {
        ID = "CONFIG";
    }
};

#endif
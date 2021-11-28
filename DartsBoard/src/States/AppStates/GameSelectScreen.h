#ifndef GAMESELECTSCREEN_H
#define GAMESELECTSCREEN_H

#include "../AppState.h"

#include "../../Resource/Resources.h"

class GameSelectScreen : public AppState {
private:
    String gameSchemeString;

public:
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;

    GameSelectScreen(GameLogic* gameLogic) : AppState(gameLogic) {
        ID = "SELECT";
    }
};

#endif
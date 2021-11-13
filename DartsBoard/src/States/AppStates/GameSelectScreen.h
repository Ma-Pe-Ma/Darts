#ifndef GAMESELECTSCREEN_H
#define GAMESELECTSCREEN_H

#include "../AppState.h"

class GameSelectScreen : public AppState {

public:
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;

    GameSelectScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "SELECT";}
};

#endif
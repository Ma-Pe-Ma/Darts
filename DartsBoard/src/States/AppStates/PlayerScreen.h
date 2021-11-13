#ifndef PLAYERSCREEN_H
#define PLAYERSCREEN_H

#include "../AppState.h"

class PlayerScreen : public AppState {
public:
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;

    PlayerScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "PLAYERS";}
};

#endif
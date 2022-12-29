#ifndef PLAYERSCREEN_H
#define PLAYERSCREEN_H

#include "../AppState.h"
#include "../../Resource/Resources.h"

class PlayerScreen : public AppState {
private:
    char* playersString;
    char* android1String;
    char* android2String;
    char* playerNrString;

public:
    void initialize() override;
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;

    PlayerScreen(GameLogic* gameLogic) : AppState(gameLogic) {
        ID = "PLAYERS";
    }
};

#endif
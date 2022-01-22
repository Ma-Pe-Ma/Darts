#ifndef PLAYERSCREEN_H
#define PLAYERSCREEN_H

#include "../AppState.h"
#include "../../Resource/Resources.h"

class PlayerScreen : public AppState {
private:
    String playersString;
    String android1String;
    String android2String;
    String playerNrString;
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
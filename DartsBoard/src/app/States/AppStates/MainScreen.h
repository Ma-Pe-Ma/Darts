#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "../AppState.h"

class MainScreen : public AppState {
public:
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;

    MainScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "MAIN";}
};

#endif
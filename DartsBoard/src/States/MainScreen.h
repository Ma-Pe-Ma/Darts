#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "../Hardware/ImageLoading.h"
#include "AppState.h"

class MainScreen : public AppState {
public:
    void Start();
    void Update(Pair);
    MainScreen(GameLogic* gameLogic) : AppState(gameLogic) {ID = "MAIN";}
    void ProcessMessage(JsonObject);
};

#endif
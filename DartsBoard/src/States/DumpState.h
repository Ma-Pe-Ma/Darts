#ifndef DUMPSTATE_H
#define DUMPSTATE_H

#include "AppState.h"

class DumpState : public AppState {
    public:
    void Start();
    void Update(Pair);
    DumpState(GameLogic* gameLogic) : AppState(gameLogic) {ID = "DUMP";}
    void ProcessMessage(JsonObject);
};

#endif
#ifndef DUMPSTATE_H
#define DUMPSTATE_H

#include "AppState.h"

class DumpState : public AppState {
public:
    void start() override;
    void update(Pair) override;
    void processMessage(JsonObject) override;
    
    DumpState(GameLogic* gameLogic) : AppState(gameLogic) {ID = "DUMP";}
};

#endif
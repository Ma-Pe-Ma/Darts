#ifndef APPSTATE_H
#define APPSTATE_H

#include "Arduino.h"
#include "State.h"
#include "../Hardware/DisplayContainer.h"
#include <ArduinoJson.h>

class GameLogic;

class AppState : public State {
protected:
    GameLogic* gameLogic;
    String ID = "";
    
public:
    AppState(GameLogic* gameLogic) {this->gameLogic = gameLogic;}

    void setGameLogic(GameLogic* gameLogic) {this->gameLogic = gameLogic;}
    virtual void processMessage(JsonObject) = 0;
    String getID() {return ID;}
    GameLogic* getGameLogic() {return gameLogic;}
};

#endif
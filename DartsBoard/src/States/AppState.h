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
    //virtual void Update(Pair); 
    void setGameLogic(GameLogic* gameLogic) {this->gameLogic = gameLogic;}
    AppState(GameLogic* gameLogic) {this->gameLogic = gameLogic;}
    virtual void ProcessMessage(JsonObject) = 0;
    String GetID() {return ID;}
    GameLogic* GetGameLogic() {return gameLogic;}
};

#endif
#include "DumpState.h"
#include "GameLogic.h"

void DumpState::Start() {

}

void DumpState::Update(Pair pair) {
    
}

void DumpState::ProcessMessage(JsonObject body) {
    unsigned long gameNr = body["GAMENR"].as<unsigned long>();
    gameLogic->gamePlayingScreen.SetGameNr(gameNr);
    
    JsonVariant players = body["PLAYERS"];
    gameLogic->ParsePlayers(players);

    String currentGameID = body["GAME"];
    DartsGame::dartsGame = DartsGame::findGameByID(currentGameID);

    JsonObject configObject = body["CONFIG"]; 

    for (int i = 0; i < DartsGame::nrOfGames; i++) {
        String name = DartsGame::games[i]->gameID;
        JsonObject gameConfigObject = configObject[name];
        DartsGame::games[i]->ProcessConfig(gameConfigObject);
    }

    gameLogic->TransitionTo(&(gameLogic->playerScreen));
}
#include "DumpState.h"
#include "GameLogic.h"

void DumpState::start() {

}

void DumpState::update(Pair pair) {
    
}

void DumpState::processMessage(JsonObject body) {
    unsigned long gameNr = body["GAMENR"].as<unsigned long>();
    gameLogic->gamePlayingScreen.setGameNr(gameNr);
    
    JsonVariant players = body["PLAYERS"];
    gameLogic->parsePlayers(players);

    String currentGameID = body["GAME"];
    DartsGame* currentGame = DartsGame::findGameByID(currentGameID);
    DartsGame::setCurrentGame(currentGame);

    JsonObject configObject = body["CONFIG"]; 

    for (int i = 0; i < DartsGame::nrOfGames; i++) {
        DartsGame* game = DartsGame::getGameByNr(i);

        String name = game->getGameID();
        JsonObject gameConfigObject = configObject[name];
        game->processConfig(gameConfigObject);
    }

    gameLogic->transitionTo(&(gameLogic->playerScreen));
}
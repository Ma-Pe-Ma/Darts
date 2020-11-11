#include "DartsGame.h"

#include "Cricket.h"
#include "RoundTheClock.h"
#include "X01.h"

int DartsGame::nrOfGames;
DartsGame** DartsGame::games;

BluetoothCommunicator* DartsGame::bluetoothCommunicator;

int DartsGame::chosenGameCursor = 0;

DartsGame* DartsGame::dartsGame;
DartsGame* DartsGame::prevSelected;

DartsGame* DartsGame::findGameByID(String gameID) {	
	for (int i = 0; i < NUMBER_OF_GAMES; i++) {
		if (games[i]->gameID == gameID) {
			return games[i];
		}
	}
	
	return nullptr;
}

void DartsGame::InitializeGames() {
	nrOfGames = NUMBER_OF_GAMES;
	games = new DartsGame*[nrOfGames];

	games[0] = new Cricket();
	games[1] = new RoundTheClock();
	games[2] = new X01();

	dartsGame = games[chosenGameCursor];
	prevSelected = dartsGame;
}

DartsGame* DartsGame::prevGame() {
	if (--chosenGameCursor < 0) {
		chosenGameCursor = NUMBER_OF_GAMES - 1;			
	}

	return games[chosenGameCursor];
}

DartsGame* DartsGame::nextGame() {
	if (++chosenGameCursor == NUMBER_OF_GAMES) {
		chosenGameCursor = 0;
	}

	return games[chosenGameCursor];
}

void DartsGame::SerializeConfig() {
	StaticJsonDocument<500> doc;
	
	doc["STATE"] = "CONFIG";
	JsonObject body = doc.createNestedObject("BODY");
	body["GAME"] = gameID;
	
	JsonObject configObject = body.createNestedObject("CONFIG");

	SerializeConfigCustom(configObject);
	
	String message;
	serializeJson(doc, message);	

	bluetoothCommunicator->send(message);
}
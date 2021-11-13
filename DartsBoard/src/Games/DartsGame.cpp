#include "DartsGame.h"

#include "Cricket.h"
#include "RoundTheClock.h"
#include "X01.h"

const int DartsGame::nrOfGames = NUMBER_OF_GAMES;
DartsGame** DartsGame::games;

BluetoothCommunicator* DartsGame::bluetoothCommunicator;

int DartsGame::chosenGameCursor = 0;

DartsGame* DartsGame::dartsGame;
DartsGame* DartsGame::prevSelected;

DartsGame* DartsGame::getGameByNr(int number) {
	return games[number];
}

DartsGame* DartsGame::getCurrentGame() {
	return DartsGame::dartsGame;
}

void DartsGame::setCurrentGame(DartsGame* dartsGame) {
	DartsGame::dartsGame = dartsGame;
}

DartsGame* DartsGame::findGameByID(String gameID) {	
	for (int i = 0; i < NUMBER_OF_GAMES; i++) {
		if (games[i]->gameID == gameID) {
			return games[i];
		}
	}
	
	return nullptr;
}

void DartsGame::initializeGames() {
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

void DartsGame::serializeConfig() {
	StaticJsonDocument<500> doc;
	
	doc["STATE"] = "CONFIG";
	JsonObject body = doc.createNestedObject("BODY");
	body["GAME"] = gameID;
	
	JsonObject configObject = body.createNestedObject("CONFIG");

	this->serializeConfigCustom(configObject);
	
	String message;
	serializeJson(doc, message);	

	bluetoothCommunicator->send(message);
}
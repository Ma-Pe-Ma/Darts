#include "GameLogic.h"

GameLogic::GameLogic(DisplayContainer* displayContainer, PlayerContainer* playerContainer, GameContainer* gameContainer) {
	this->displayContainer = displayContainer;
	this->playerContainer = playerContainer;
	this->gameContainer = gameContainer;
}

void GameLogic::init() {
	states = new AppState*[NR_OF_APPSTATES];
	states[0] = &mainScreen;
	states[1] = &playerScreen;
	states[2] = &gameSelectScreen;
	states[3] = &gameConfiguringScreen;
	states[4] = &customGameConfiguringScreen;
	states[5] = &gamePlayingScreen;

	currentState = states[0];

	for (int i = 0; i < NR_OF_APPSTATES; i++) {
		states[i]->initialize();
	}
}

AppState* GameLogic::findStateByID(String ID) {
	for (int i = 0; i < NR_OF_APPSTATES; i++) {
		if (states[i]->getID() == ID) {
			return states[i];
		}
	}
	
	return nullptr;
}

void GameLogic::run() {
	//detecting touch inputs
	Pair touch = displayContainer->getTouchedPoint();
	currentState->update(touch);
}

void GameLogic::receiveMessage(String message) {
	StaticJsonDocument<2048> doc;
	DeserializationError err = deserializeJson(doc, message);

	if (err == DeserializationError::Ok) {
		JsonObject jsonObject = doc.as<JsonObject>();
		
		String stateHeader = jsonObject["STATE"].as<String>();

		AppState* newState = findStateByID(stateHeader);
		JsonObject body = jsonObject["BODY"].as<JsonObject>();

		if (stateHeader == "DUMP") {
			processDump(body);
			return;
		}

		if(stateHeader == "LAST") {
			bluetoothCommunicator->repeatLastMessage();
			return;
		}

		newState->processMessage(body);

		if (newState != currentState) {
			this->transitionTo(newState);
		}
	}
	else {
		String error = err.c_str();
		Serial.println("Error: "+error);
	}	
}

void GameLogic::parsePlayers(JsonVariant players) {
	for (int i = 0; i < players.size(); i++) {
		JsonObject player = players[String(i + 1)];

		Player* playerObject = playerContainer->getPlayerByNumber(i);
		
		uint16_t color = Player::convertColor(player["COLOR"].as<long>());;
		uint16_t inverseColor = Player::convertColor(player["ICOLOR"].as<long>());;
		//uint16_t inverseColor = 0xffff - color;

		playerObject->setColor(color);
		playerObject->setInverseColor(inverseColor);
		playerObject->setNick(player["NICK"].as<String>());
		playerObject->setName(player["NAME"].as<String>());
	}
	
	playerContainer->setNumberOfPlayers(players.size());
}

void GameLogic::processDump(JsonObject body) {
    unsigned long gameNr = body["GAMENR"].as<unsigned long>();
    this->gamePlayingScreen.setGameNr(gameNr);
    
    JsonVariant players = body["PLAYERS"];
    this->parsePlayers(players);

    String currentGameID = body["GAME"];
    DartsGame* currentGame = gameContainer->findGameByID(currentGameID);
	gameContainer->setCurrentGame(currentGame);

    JsonObject configObject = body["CONFIG"]; 

    for (int i = 0; i < gameContainer->nrOfGames; i++) {
        DartsGame* game = gameContainer->getGameByNr(i);

        String name = game->getGameID();
        JsonObject gameConfigObject = configObject[name];
        game->processConfig(gameConfigObject);
    }

    this->transitionTo(&playerScreen);
}

//used to ask for dump from client, only used at startup
void GameLogic::askConfigDump() {
	StaticJsonDocument<1024> doc;
	doc["STATE"] = "DUMP";
	doc.createNestedObject("BODY");

	String message;
	serializeJson(doc, message);

	bluetoothCommunicator->send(message);
}

void GameLogic::notifyAboutStart() {
	StaticJsonDocument<1024> doc;
	doc["STATE"] = "GAMEPLAY";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "START";
	
	JsonObject gBody = body.createNestedObject("GBODY");
	gBody["ORDER"] = orderModify;

	String message;
	serializeJson(doc, message);
	
	bluetoothCommunicator->send(message);
}
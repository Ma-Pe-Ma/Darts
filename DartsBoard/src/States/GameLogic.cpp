#include "GameLogic.h"

GameLogic GameLogic::gameLogic;

void GameLogic::staticProcessMessage(String message) {
	gameLogic.processMessage(message);
}

GameLogic::GameLogic() {
	states = new AppState*[NR_OF_APPSTATES];
	states[0] = &mainScreen;
	states[1] = &playerScreen;
	states[2] = &gameSelectScreen;
	states[3] = &gameConfiguringScreen;
	states[4] = &customGameConfiguringScreen;
	states[5] = &gamePlayingScreen;

	states[6] = &dumpState;
	states[7] = &lastState;

	currentState = states[0];
}

AppState* GameLogic::findStateByID(String ID) {
	for (int i = 0; i < NR_OF_APPSTATES; i++) {
		if (states[i]->getID() == ID) {
			return states[i];
		}
	}
	
	return nullptr;
}

void GameLogic::processMessage(String message) {
	StaticJsonDocument<2048> doc;
	DeserializationError err = deserializeJson(doc, message);

	if (err == DeserializationError::Ok) {
		JsonObject jsonObject = doc.as<JsonObject>();
		
		AppState* newState = findStateByID(jsonObject["STATE"].as<String>());
		JsonObject body = jsonObject["BODY"].as<JsonObject>();

		newState->processMessage(body);
	}
	else {
		String error = err.c_str();
		Serial.println("Error: "+error);
	}	
}

void GameLogic::parsePlayers(JsonVariant players) {
	for (int i = 0; i < players.size(); i++) {
		JsonObject player = players[String(i + 1)];

		Player* playerObject = Player::getPlayerByNumber(i);
		
		uint16_t color = Player::convertColor(player["COLOR"].as<long>());;
		uint16_t inverseColor = Player::convertColor(player["ICOLOR"].as<long>());;
		//uint16_t inverseColor = 0xffff - color;

		playerObject->setColor(color);
		playerObject->setInverseColor(inverseColor);
		playerObject->setNick(player["NICK"].as<String>());
		playerObject->setName(player["NAME"].as<String>());
	}
	
	Player::setNumberOfPlayers(players.size());
}

void GameLogic::getConfigDump() {
	StaticJsonDocument<1024> doc;
	doc["STATE"] = "DUMP";
	doc.createNestedObject("BODY");

	String message;
	serializeJson(doc, message);
	
	bluetoothCommunicator->send(message);
}

void GameLogic::run(Pair touch) {
	currentState->update(touch);
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
#include "GameLogic.h"

GameLogic GameLogic::gameLogic;

void GameLogic::StaticProcessMessage(String message) {
	gameLogic.ProcessMessage(message);
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

AppState* GameLogic::FindStateByID(String ID) {
	for (int i = 0; i < NR_OF_APPSTATES; i++) {
		if (states[i]->GetID() == ID) {
			return states[i];
		}
	}
	
	return nullptr;
}

void GameLogic::ProcessMessage(String message) {
	StaticJsonDocument<2048> doc;
	DeserializationError err = deserializeJson(doc, message);

	if (err == DeserializationError::Ok) {
		JsonObject jsonObject = doc.as<JsonObject>();
		
		AppState* newState = FindStateByID(jsonObject["STATE"].as<String>());
		JsonObject body = jsonObject["BODY"].as<JsonObject>();

		newState->ProcessMessage(body);
	}
	else {
		String error = err.c_str();
		Serial.println("Error: "+error);
	}	
}

void GameLogic::ParsePlayers(JsonVariant players) {
	for (int i = 0; i < players.size(); i++) {
		JsonObject player = players["" + String(i + 1)];
		Player::players[i].color = Player::convertColor(player["COLOR"].as<long>());
		Player::players[i].inverseColor = 0xffff - Player::players[i].color;
		Player::players[i].nickname = player["NICK"].as<String>();
		Player::players[i].name = player["NAME"].as<String>();
	}
				
	Player::number = players.size();	
}

void GameLogic::GetConfigDump() {
	StaticJsonDocument<1024> doc;
	doc["STATE"] = "DUMP";
	doc.createNestedObject("BODY");

	String message;
	serializeJson(doc, message);
	
	bluetoothCommunicator->send(message);
}

void GameLogic::Run(Pair touch) {
	currentState->Update(touch);
}

void GameLogic::NotifyAboutStart() {
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
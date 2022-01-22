#include "DartsGame.h"

#include "Cricket.h"
#include "RoundTheClock.h"
#include "X01.h"

void DartsGame::serializeConfig() {
	StaticJsonDocument<500> doc;
	
	doc["STATE"] = "CONFIG";
	JsonObject body = doc.createNestedObject("BODY");
	body["GAME"] = gameID;
	
	JsonObject configObject = body.createNestedObject("CONFIG");

	this->serializeConfigCustom(configObject);
	
	String message;
	serializeJson(doc, message);	

	//bluetoothCommunicator->send(message);
}
#include "GamePlayingScreen.h"
#include "../GameLogic.h"

GamePlayingScreen::GamePlayingScreen(GameLogic* gameLogic) : AppState(gameLogic) {
    ID = "PLAY";
	currentState = &intro;
}

void GamePlayingScreen::start() {
    getGameLogic()->displayContainer->getTFT()->fillScreen(CYAN);
	
	int buttonSize = SCR_WIDTH / 10;
	int squareOffset = int(SCR_WIDTH * 0.08f);

    gameLogic->nextMenu.setImage(getGameLogic()->displayContainer->getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, RED, GREEN, MAGENTA, "P", 3);	

    gameLogic->delete1.setImage(getGameLogic()->displayContainer->getTFT(), dartStatusStartX + 0 * dartStatusOffsetX, dartStatusStartY, dartStatusWidth, dartStatusHeight, WHITE, CYAN, BLACK, "del1", 2);
    gameLogic->delete2.setImage(getGameLogic()->displayContainer->getTFT(), dartStatusStartX + 1 * dartStatusOffsetX, dartStatusStartY, dartStatusWidth, dartStatusHeight, WHITE, CYAN, BLACK, "del2", 2);
    gameLogic->delete3.setImage(getGameLogic()->displayContainer->getTFT(), dartStatusStartX + 2 * dartStatusOffsetX, dartStatusStartY, dartStatusWidth, dartStatusHeight, WHITE, CYAN, BLACK, "del3", 2);

    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);

	//setting the proper order, if it's the first start then it's unchanged
    gameLogic->playerContainer->setCorrectPlayerOrder(gameLogic->orderModify);
    
    roundCounter = 0;

	gameNr++;

	DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();

    //setting proper score objects to players!
    for (int i = 0; i < gameLogic->playerContainer->getNumberOfPlayers(); i++) {
		Player* checkable = gameLogic->playerContainer->getPlayerByNumber(i);

		if (checkable->getScore() != nullptr) {
			delete checkable->getScore();
        }
        
        checkable->setScore(currentGame->setProperScoreContainer());
		checkable->getScore()->setPosition(-1);
    }

    currentGame->initializeGame();

	this->sendStart();
	this->transitionTo(&intro);
}

void GamePlayingScreen::update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);

	if (gameLogic->nextMenu.released()) {
		gameLogic->nextMenu.guiButton.drawButton(true);
	}

	if (gameLogic->prevMenu.simple()) {
		gameLogic->transitionTo(&gameLogic->gameConfiguringScreen);
	}

	gameLogic->delete1.detect(touch);
	gameLogic->delete2.detect(touch);
	gameLogic->delete3.detect(touch);

	if (gameLogic->delete1.simple()) {
		this->invertDart(0);
	}

	if (gameLogic->delete2.simple()) {
		this->invertDart(1);
	}
	
	if (gameLogic->delete3.simple()) {
		this->invertDart(2);
	}

	currentState->update(touch);
}

void GamePlayingScreen::invertDart(int position) {
	if (position < boardContainer.getCurrentDartID()) {
		del[position] = !del[position];

		Sector dart = boardContainer.getThrownDartByNumber(position);
		Player* currentPlayer = gameLogic->playerContainer->getCurrentPlayer();

		if (del[position]) {
			currentPlayer->getScore()->deleteThrow(dart);
			getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, RED, 2, "      ");
		}
		else {
			currentPlayer->getScore()->scoreThrow(dart);
			String text = String(position + 1) + ": " + DisplayContainer::sectorText(dart);
			getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, CYAN, 2, "      ");
			getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, CYAN, 2, text);
		}
	}	

	this->sendRoundDump();	
}

void GamePlayingScreen::correct() {
	if (boardContainer.getCurrentDartID() > 0) {
		int newID = boardContainer.getCurrentDartID() - 1;
		boardContainer.setCurrentDartID(newID);
		
		Sector currentDart = boardContainer.getThrownDartByNumber(newID);
		boardContainer.setThrownDartByNumber(newID, {.base = 0, .multiplier = 0});
		
		gameLogic->playerContainer->getCurrentPlayer()->getScore()->correct(currentDart);
		String text = String(boardContainer.getCurrentDartID() + 1) + ": " + DisplayContainer::sectorText(currentDart);
		getGameLogic()->displayContainer->writeWithBackground(dartStatusStartX + dartStatusOffsetX * boardContainer.getCurrentDartID(), dartStatusStartY, BLACK, CYAN, 2, text);

		if (currentState != &throwing) {
			this->transitionTo(&throwing);
		}
	}

	this->sendRoundDump();
}

void GamePlayingScreen::createPlayerDump(JsonObject& playerBody, Player* player) {
	playerBody["NICK"] = player->getNick();
	playerBody["SCORE"] = player->getScore()->getPlayerScore();
	playerBody["ROUND"] = player->getScore()->getRoundCounter();
	playerBody["POSITION"] = player->getScore()->getPosition();
	JsonObject gameStatus = playerBody.createNestedObject("STATUS");
	player->getScore()->serializePlayerStatus(gameStatus);
}

//process messages from client
void GamePlayingScreen::processMessage(JsonObject message) {
    String gState = message["GSTATE"];
	JsonObject gBody = message["GBODY"];

	if (gState == "START") {
		gameLogic->orderModify = static_cast<OrderModify>(gBody["ORDER"].as<int>());
		gameLogic->transitionTo(this);
	}

	if (gState == "NEXT") {
		onNext(gBody);
	}
	
	if (gState == "DEL") {
		onDelete(gBody);
	}

	if (gState == "CORRECT") {
		onCorrect(gBody);
	}

	if (gState == "DUMP") {
		onDump(gBody);
	}
}

void GamePlayingScreen::onNext(JsonObject& body) {
	this->transitionTo(&intro);
}

void GamePlayingScreen::onDump(JsonObject& body) {
	this->sendDump();
	this->sendRoundDump();
}

void GamePlayingScreen::onDelete(JsonObject& body) {
	int dartID = body["DEL"].as<int>();
	this->invertDart(dartID);
	this->sendRoundDump();
}

void GamePlayingScreen::onCorrect(JsonObject& body) {
	this->correct();
	this->sendRoundDump();
}

//responses
void GamePlayingScreen::sendStart() {
	StaticJsonDocument<10240> doc;
	
	doc["STATE"] = "GAME";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "START";
	JsonObject gBody = body.createNestedObject("GBODY");

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}

void GamePlayingScreen::sendDump() {
	StaticJsonDocument<10240> doc;

	doc["STATE"] = "GAME";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "DUMP";
	JsonObject gBody = body.createNestedObject("GBODY");

	DartsGame* currentGame = gameLogic->gameContainer->getCurrentGame();
	gBody["GAME"] = currentGame->getGameID();
	gBody["CURRENT"] = gameLogic->playerContainer->getCurrentPlayer()->getNick();

	//gBody["FINISHED"] = gameLogic->playerContainer->getNumberOfPlayers() == gameLogic->playerContainer->getNumberOfFinishedPlayers();
	gBody["GAMENR"] = gameNr;

	JsonObject config = gBody.createNestedObject("CONFIG");
	currentGame->serializeConfigCustom(config);

	JsonObject players = gBody.createNestedObject("PLAYERS");

	for (int i = 0; i < gameLogic->playerContainer->getNumberOfPlayers(); i++) {
		JsonObject player = players.createNestedObject(String(i));
		createPlayerDump(player, gameLogic->playerContainer->getPlayerByNumber(i));
	}

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}

void GamePlayingScreen::sendRoundDump() {
	StaticJsonDocument<4096> doc;
	doc["STATE"] = "GAMEPLAY";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "ROUND";
	JsonObject gBody = body.createNestedObject("GBODY");

	gBody["ROUND"] = roundCounter;
	gBody["DART"] = boardContainer.getCurrentDartID();
	JsonObject player = gBody.createNestedObject("PLAYER");

	createPlayerDump(player, gameLogic->playerContainer->getCurrentPlayer());

	JsonObject dartBody = gBody.createNestedObject("DARTS");

	for (int i = 0 ; i < 3; i++) {
		JsonObject dart = dartBody.createNestedObject(String(i));
		Sector dartObject = boardContainer.getThrownDartByNumber(i);

		dart["M"] = dartObject.multiplier;
		dart["S"] = dartObject.base;
		dart["DEL"] = del[i];
	}

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}

void GamePlayingScreen::sendHit(Sector sector) {
	StaticJsonDocument<500> doc;
	doc["STATE"] = "GAMEPLAY";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "HIT";
	JsonObject gBody = body.createNestedObject("GBODY");

	int dartID = boardContainer.getCurrentDartID();
	Sector dart = boardContainer.getThrownDartByNumber(dartID);

	gBody["DART"] = dartID;
	gBody["M"] = dart.multiplier;
	gBody["S"] = dart.base;
	gBody["SCORE"] = gameLogic->playerContainer->getCurrentPlayer()->getScore()->getPlayerScore();
	gBody["ROUND"] = gameLogic->playerContainer->getCurrentPlayer()->getScore()->getRoundCounter();

	JsonObject dartState = gBody.createNestedObject("DSTATE");
	gameLogic->playerContainer->getCurrentPlayer()->getScore()->serializeDartStatus(dartState, sector);

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}
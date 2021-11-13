#include "GamePlayingScreen.h"
#include "GameLogic.h"

GamePlayingScreen::GamePlayingScreen(GameLogic* gameLogic) : AppState(gameLogic) {
    ID = "PLAY";
	currentState = &intro;
}

void GamePlayingScreen::start() {
    DisplayContainer::displayContainer.getTFT()->fillScreen(CYAN);
	
	int buttonSize = SCR_WIDTH / 10;
	int squareOffset = int(SCR_WIDTH * 0.08f);

    gameLogic->nextMenu.setImage(DisplayContainer::displayContainer.getTFT(), SCR_WIDTH - squareOffset, squareOffset, buttonSize, buttonSize, RED, GREEN, MAGENTA, "P", 3);	

    gameLogic->delete1.setImage(DisplayContainer::displayContainer.getTFT(), dartStatusStartX + 0 * dartStatusOffsetX, dartStatusStartY, dartStatusWidth, dartStatusHeight, WHITE, CYAN, BLACK, "del1", 2);
    gameLogic->delete2.setImage(DisplayContainer::displayContainer.getTFT(), dartStatusStartX + 1 * dartStatusOffsetX, dartStatusStartY, dartStatusWidth, dartStatusHeight, WHITE, CYAN, BLACK, "del2", 2);
    gameLogic->delete3.setImage(DisplayContainer::displayContainer.getTFT(), dartStatusStartX + 2 * dartStatusOffsetX, dartStatusStartY, dartStatusWidth, dartStatusHeight, WHITE, CYAN, BLACK, "del3", 2);

    gameLogic->prevMenu.guiButton.drawButton(true);
    gameLogic->nextMenu.guiButton.drawButton(true);

	//setting the proper order, if it's the first start then it's unchanged
    Player::setCorrectPlayerOrder();
    
    roundCounter = 0;

	gameNr++;

	DartsGame* currentGame = DartsGame::getCurrentGame();

    //setting proper score objects to players!
    for (int i = 0; i < Player::getNumberOfPlayers(); i++) {
		Player* checkable = Player::getPlayerByNumber(i);

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
	if (position < BoardContainer::currentDart) {
		del[position] = !del[position];

		Sector dart = BoardContainer::darts[position];

		Player* currentPlayer = Player::getCurrentPlayer();

		if (del[position]) {
			currentPlayer->getScore()->deleteThrow(dart);
			DisplayContainer::displayContainer.writeWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, RED, 2, "      ");
		}
		else {
			currentPlayer->getScore()->scoreThrow(dart);
			String text = String(position + 1) + ": " + DisplayContainer::sectorText(BoardContainer::darts[position]);
			DisplayContainer::displayContainer.writeWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, CYAN, 2, "      ");
			DisplayContainer::displayContainer.writeWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, CYAN, 2, text);
		}
	}	

	this->sendRoundDump();	
}

void GamePlayingScreen::correct() {
	if (BoardContainer::currentDart > 0) {		
		BoardContainer::boardContainer.currentDart--;		
		Player::getCurrentPlayer()->getScore()->correct(BoardContainer::darts[BoardContainer::currentDart]);
		String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::sectorText(BoardContainer::darts[BoardContainer::currentDart]);
		DisplayContainer::displayContainer.writeWithBackground(dartStatusStartX + dartStatusOffsetX * BoardContainer::currentDart, dartStatusStartY, BLACK, CYAN, 2, text);
		
		BoardContainer::darts[BoardContainer::currentDart] = {.base = 0, .multiplier = 0};

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

//callbacks
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

	DartsGame* currentGame = DartsGame::getCurrentGame();
	gBody["GAME"] = currentGame->getGameID();
	gBody["CURRENT"] = Player::getCurrentPlayer()->getNick();

	//gBody["FINISHED"] = Player::getNumberOfPlayers() == Player::getNumberOfFinishedPlayers();
	gBody["GAMENR"] = gameNr;

	JsonObject config = gBody.createNestedObject("CONFIG");
	currentGame->serializeConfigCustom(config);

	JsonObject players = gBody.createNestedObject("PLAYERS");

	for (int i = 0; i < Player::getNumberOfPlayers(); i++) {
		JsonObject player = players.createNestedObject(String(i));
		createPlayerDump(player, Player::getPlayerByNumber(i));
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
	gBody["DART"] = BoardContainer::currentDart;
	JsonObject player = gBody.createNestedObject("PLAYER");

	createPlayerDump(player, Player::getCurrentPlayer());

	JsonObject dartBody = gBody.createNestedObject("DARTS");

	for (int i = 0 ; i < 3; i++) {
		JsonObject dart = dartBody.createNestedObject(String(i));
		dart["M"] = BoardContainer::darts[i].multiplier;
		dart["S"] = BoardContainer::darts[i].base;
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

	gBody["DART"] = BoardContainer::currentDart;
	gBody["M"] = BoardContainer::darts[BoardContainer::currentDart].multiplier;
	gBody["S"] = BoardContainer::darts[BoardContainer::currentDart].base;
	gBody["SCORE"] = Player::getCurrentPlayer()->getScore()->getPlayerScore();
	gBody["ROUND"] = Player::getCurrentPlayer()->getScore()->getRoundCounter();

	JsonObject dartState = gBody.createNestedObject("DSTATE");
	Player::getCurrentPlayer()->getScore()->serializeDartStatus(dartState, sector);

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}
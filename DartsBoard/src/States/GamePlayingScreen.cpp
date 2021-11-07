#include "GamePlayingScreen.h"
#include "GameLogic.h"

GamePlayingScreen::GamePlayingScreen(GameLogic* gameLogic) : AppState(gameLogic) {
    ID = "PLAY";
	currentState = &intro;
}

void GamePlayingScreen::Start() {
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
    Player::SetCorrectPlayerOrder();

    Player::cursor = Player::number - 1;
    roundCounter = 0;

	nextWinningPosition = 1;
	gameFinished = false;
	gameNr++;

    //setting proper score objects to players!
    for (int i = 0; i < Player::number; i++) {
        if (Player::players[i].score != nullptr) {
			delete Player::players[i].score;
        }
        
        Player::players[i].score = DartsGame::dartsGame->SetProperScoreContainer();
		Player::players[i].score->position = -1;
    }

    DartsGame::dartsGame->InitializeGame();

	intro.firstTime = true;
	TransitionTo(&intro);
	SendStart();
}

void GamePlayingScreen::Update(Pair touch) {
    gameLogic->prevMenu.detect(touch);
	gameLogic->nextMenu.detect(touch);

	if (gameLogic->nextMenu.released()) {
		gameLogic->nextMenu.guiButton.drawButton(true);
	}

	if (gameLogic->prevMenu.simple()) {
		gameLogic->TransitionTo(&gameLogic->gameConfiguringScreen);
	}

	gameLogic->delete1.detect(touch);
	gameLogic->delete2.detect(touch);
	gameLogic->delete3.detect(touch);

	if (gameLogic->delete1.simple()) {
		InvertDart(0);
	}

	if (gameLogic->delete2.simple()) {
		InvertDart(1);
	}
	
	if (gameLogic->delete3.simple()) {
		InvertDart(2);
	}

	currentState->Update(touch);
}

void GamePlayingScreen::InvertDart(int position) {
	if (position < BoardContainer::currentDart) {
		del[position] = !del[position];

		Sector dart = BoardContainer::darts[position];

		if (del[position]) {
			Player::current->score->Delete(dart);
			DisplayContainer::displayContainer.WriteWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, RED, 2, "      ");
		}
		else {
			Player::current->score->Score(dart);
			String text = String(position + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[position]);
			DisplayContainer::displayContainer.WriteWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, CYAN, 2, "      ");
			DisplayContainer::displayContainer.WriteWithBackground(dartStatusStartX + dartStatusOffsetX * position, dartStatusStartY, BLACK, CYAN, 2, text);
		}
		
	}	

	SendRoundDump();	
}

void GamePlayingScreen::Correct() {
	if (BoardContainer::currentDart > 0) {		
		BoardContainer::boardContainer.currentDart--;		
		Player::current->score->Correct(BoardContainer::darts[BoardContainer::currentDart]);
		String text = "" + String(BoardContainer::currentDart + 1) + ": " + DisplayContainer::SectorText(BoardContainer::darts[BoardContainer::currentDart]);
		DisplayContainer::displayContainer.WriteWithBackground(dartStatusStartX + dartStatusOffsetX * BoardContainer::currentDart, dartStatusStartY, BLACK, CYAN, 2, text);
		
		BoardContainer::darts[BoardContainer::currentDart] = {.base = 0, .multiplier = 0};

		if (currentState != &throwing) {
			TransitionTo(&throwing);
		}
	}

	SendRoundDump();
}

void GamePlayingScreen::CreatePlayerDump(JsonObject& playerBody, Player* player) {
	playerBody["NICK"] = player->nickname;
	playerBody["SCORE"] = player->score->playerScore;
	playerBody["ROUND"] = player->score->roundCounter;
	playerBody["POSITION"] = player->score->position;
	JsonObject gameStatus = playerBody.createNestedObject("STATUS");
	player->score->SerializePlayerStatus(gameStatus);
}

void GamePlayingScreen::ProcessMessage(JsonObject message) {
    String gState = message["GSTATE"];
	JsonObject gBody = message["GBODY"];

	if (gState == "START") {
		gameLogic->orderModify = static_cast<OrderModify>(gBody["ORDER"].as<int>());
		gameLogic->TransitionTo(this);
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
	TransitionTo(&intro);
}

void GamePlayingScreen::onDump(JsonObject& body) {
	SendDump();
	SendRoundDump();
}

void GamePlayingScreen::onDelete(JsonObject& body) {
	int dartID = body["DEL"].as<int>();
	InvertDart(dartID);

	SendRoundDump();
}

void GamePlayingScreen::onCorrect(JsonObject& body) {
	Correct();
	SendRoundDump();
}

//responses
void GamePlayingScreen::SendStart() {
	StaticJsonDocument<10240> doc;
	
	doc["STATE"] = "GAME";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "START";
	JsonObject gBody = body.createNestedObject("GBODY");

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}

void GamePlayingScreen::SendDump() {
	StaticJsonDocument<10240> doc;
	
	doc["STATE"] = "GAME";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "DUMP";
	JsonObject gBody = body.createNestedObject("GBODY");

	gBody["GAME"] = DartsGame::dartsGame->gameID;
	gBody["CURRENT"] = Player::current->nickname;

	//gBody["FINISHED"] = gameFinished;
	gBody["GAMENR"] = gameNr;

	JsonObject config = gBody.createNestedObject("CONFIG");
	DartsGame::dartsGame->SerializeConfigCustom(config);

	JsonObject players = gBody.createNestedObject("PLAYERS");

	for (int i = 0; i < Player::number; i++) {
		JsonObject player = players.createNestedObject(String(i));
		CreatePlayerDump(player, &Player::players[i]);
	}

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}

void GamePlayingScreen::SendRoundDump() {
	StaticJsonDocument<4096> doc;
	doc["STATE"] = "GAMEPLAY";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "ROUND";
	JsonObject gBody = body.createNestedObject("GBODY");

	gBody["ROUND"] = roundCounter;
	gBody["DART"] = BoardContainer::currentDart;
	JsonObject player = gBody.createNestedObject("PLAYER");

	CreatePlayerDump(player, Player::current);

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

void GamePlayingScreen::SendHit(Sector sector) {
	StaticJsonDocument<500> doc;
	doc["STATE"] = "GAMEPLAY";
	JsonObject body = doc.createNestedObject("BODY");

	body["GSTATE"] = "HIT";
	JsonObject gBody = body.createNestedObject("GBODY");

	gBody["DART"] = BoardContainer::currentDart;
	gBody["M"] = BoardContainer::darts[BoardContainer::currentDart].multiplier;
	gBody["S"] = BoardContainer::darts[BoardContainer::currentDart].base;
	gBody["SCORE"] = Player::current->score->playerScore;
	gBody["ROUND"] = Player::current->score->roundCounter;

	JsonObject dartState = gBody.createNestedObject("DSTATE");
	Player::current->score->SerializeDartStatus(dartState, sector);

	String message;
	serializeJson(doc, message);
	
	gameLogic->bluetoothCommunicator->send(message);
}
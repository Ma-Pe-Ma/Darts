#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "../Games/DartsGame.h"

#include "../Hardware/ImageLoading.h"
#include "../Hardware/BoardContainer.h"
#include "../Hardware/Pair.h"
#include "../Hardware/Button.h"
#include "../Hardware/BluetoothCommunicator.h"

#include "../SimpleMap.h"

#include "StateContext.h"

#include "MainScreen.h"
#include "PlayerScreen.h"
#include "GameSelectScreen.h"
#include "GameConfiguringScreen.h"
#include "CustomGameConfiguringScreen.h"
#include "GamePlayingScreen.h"

#include "DumpState.h"
#include "LastState.h"

#define NR_OF_APPSTATES 8

enum OrderModify {
	unchanged = 0,
	cyclical = 1,
	reversed = 2
};

class GameLogic : public StateContext {
	AppState* FindStateByID(String);
	
public:
	static GameLogic gameLogic;
	GameLogic();
	bool androidMode = false;	

	//Possible scenes
	MainScreen mainScreen = MainScreen(this);
	PlayerScreen playerScreen = PlayerScreen(this);
	GameSelectScreen gameSelectScreen = GameSelectScreen(this);
	GameConfiguringScreen gameConfiguringScreen = GameConfiguringScreen(this);
	CustomGameConfiguringScreen customGameConfiguringScreen = CustomGameConfiguringScreen(this);
	GamePlayingScreen gamePlayingScreen = GamePlayingScreen(this);

	//fake scenes
	DumpState dumpState = DumpState(this);
	LastState lastState = LastState(this);

	AppState** states;

	Button nextCursor, prevCursor, middleCursor;
	Button nextMenu, prevMenu;
	Button offline, android;
	Button delete1, delete2, delete3;

	void GetDump();
	void Run(Pair);

	void ParsePlayers(JsonVariant);
	
	static void StaticProcessMessage(String);
	void ProcessMessage(String);

	void NotifyAboutStart();
	
	BluetoothCommunicator* bluetoothCommunicator;
	OrderModify orderModify = unchanged;

	void SetCorrectPlayerOrder();
};

#endif 
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "../Games/DartsGame.h"

#include "../Hardware/BoardContainer.h"
#include "../Hardware/Pair.h"
#include "../Hardware/Button.h"
#include "../Hardware/Bluetooth/BluetoothCommunicator.h"
#include "../Hardware/Bluetooth/MessageReceiver.h"

#include "../SimpleMap.h"

#include "StateContext.h"

#include "AppStates/MainScreen.h"
#include "AppStates/PlayerScreen.h"
#include "AppStates/GameSelectScreen.h"
#include "AppStates/GameConfiguringScreen.h"
#include "AppStates/CustomGameConfiguringScreen.h"
#include "AppStates/GamePlayingScreen.h"

#include "../Games/GameContainer.h"
#include "../Player/PlayerContainer.h"
#include "../Hardware/DisplayContainer.h"
#include "../Games/OrderModify.h"

#define NR_OF_APPSTATES 6

class GameLogic : public StateContext, public MessageReceiver {
	AppState* findStateByID(String);
	AppState** states;	
public:
	GameLogic(DisplayContainer* displayContainer, PlayerContainer* playerContainer, GameContainer* gameContainer);
	void init();
	bool androidMode = false;	

	//Possible scenes
	MainScreen mainScreen = MainScreen(this);
	PlayerScreen playerScreen = PlayerScreen(this);
	GameSelectScreen gameSelectScreen = GameSelectScreen(this);
	GameConfiguringScreen gameConfiguringScreen = GameConfiguringScreen(this);
	CustomGameConfiguringScreen customGameConfiguringScreen = CustomGameConfiguringScreen(this);
	GamePlayingScreen gamePlayingScreen = GamePlayingScreen(this);

	//gui buttons mainly for configuring
	Button nextCursor, prevCursor, middleCursor;
	Button nextMenu, prevMenu;
	Button offline, android;

	//buttons used to delete invalid throwing
	Button delete1, delete2, delete3;

	//main logic receiving touch input
	void run();

	//receiving annd processing
	void receiveMessage(String) override;
	void parsePlayers(JsonVariant);
	void processDump(JsonObject);

	//sending messages
	void askConfigDump();
	void notifyAboutStart();
	
	BluetoothCommunicator* bluetoothCommunicator;
	OrderModify orderModify = unchanged;

	PlayerContainer* playerContainer;
	DisplayContainer* displayContainer;
	GameContainer* gameContainer;
};

#endif 
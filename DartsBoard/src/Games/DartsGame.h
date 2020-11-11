#ifndef DARTSGAME_H
#define DARTSGAME_H

#include "Arduino.h"
#include "../Hardware/Sector.h"
#include "../Player.h"
#include "../Hardware/BluetoothCommunicator.h"
#include "../Hardware/DisplayContainer.h"
#include "../Hardware/Pair.h"
#include "../Hardware/Button.h"
#include "KeyPad.h"
#include <ArduinoJson.h>

#define NUMBER_OF_GAMES 3

class DartsGame {	
	public:
		static int chosenGameCursor;
		static BluetoothCommunicator* bluetoothCommunicator;	
	
		String gameID = "";
		String name = "";
	
		void SerializeConfig();
		virtual void SerializeConfigCustom(JsonObject&) = 0;
		virtual void ProcessConfig(JsonObject&) = 0;
	
		virtual void GameConfig(Pair) = 0;
		virtual void PreConfig() = 0;
			
		virtual bool PreCustom() = 0;
		virtual bool CustomConfig(Pair) {}
		
		virtual void InitializeGame() = 0;
		virtual void InitializeRound() {}
		virtual AbstractScore* SetProperScoreContainer() { return nullptr;}
		
		static int nrOfGames;
		static DartsGame** games;
		static void InitializeGames();
		
		static DartsGame* dartsGame;
		static DartsGame* prevSelected;
		
		static DartsGame* findGameByID(String);

		static DartsGame* prevGame();
		static DartsGame* nextGame();
};
#endif
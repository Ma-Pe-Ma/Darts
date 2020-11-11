#ifndef CRICKET_H
#define CRICKET_H

#include "DartsGame.h"
#include "../Scores/CricketScore.h"
#include "CricketEnums.h"

#include "../SimpleMap.h"

class Cricket : public DartsGame {
	
	private:
		Button scoreButton, noscoreButton, cutthroatButton;
		Button classicButton, allButton, customButton;
		Button intervalButton, randomIntervalButton, chaoticButton;
		Button prevStart, nextStart;
		Button prevNr, nextNr;
		
		CricketType cricketType = score;
		CricketNumberSet cricketNumberSet = classicNumbers;
		CricketCustomSet cricketCustomSet = interval;
		
		void DrawCricketStart();
		void DrawCricketNr();

		void ProcessConfig(ArduinoJson::JsonObject);
		void ProcessGamePlay(ArduinoJson::JsonObject);

		uint8_t cricketNr = 6;
		uint8_t cricketStart = 15;
		const int defCricketMap[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

		int cricketMap[20] = {};
		void Delete(Sector);
	
		static SimpleMap<CricketType, String>* typeMap;
		static SimpleMap<CricketNumberSet, String>* setMap;
		static SimpleMap<CricketCustomSet, String>* customMap;
		
	public:
		void SerializeConfigCustom(JsonObject& configObject);
		void ProcessConfig(JsonObject&);
	
		void PreConfig();
		void GameConfig(Pair);
		
		bool PreCustom();
		bool CustomConfig(Pair);
		void InitializeGame();
		void InitializeRound();
		AbstractScore* SetProperScoreContainer() {
			return new CricketScore();
		}
			
		static void InitializeMaps();
		Cricket();
};

#endif
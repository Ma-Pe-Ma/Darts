#ifndef ROUNDTHECLOCK_H
#define ROUNDTHECLOCK_H

#include "DartsGame.h"

class RoundTheClock : public DartsGame {
	
	Sector randomDart;
	
	private:
		uint16_t roundRef[8];
		void Delete(Sector);
	
	public:
		void SerializeConfigCustom(JsonObject&);
		void ProcessConfig(JsonObject&);
	
		void PreConfig();
		void GameConfig(Pair);
		
		bool PreCustom();
		bool CustomConfig(Pair);
		
		void CalculateScore(Sector);
		void Status();
		void Correct();
		
		void InitializeGame() {}
		void InitializeRound();
		
		RoundTheClock() {
			gameID = "RTC";
			name = "Round The Clock";
		}

		float GetAverageScore();
		void SerializeGameConfig(JsonObject&);
};

#endif
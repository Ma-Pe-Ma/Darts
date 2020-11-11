#ifndef X01_H
#define X01_H

#include "DartsGame.h"

class X01 : public DartsGame {
	
	private:
		int startingValue;
		int busted;
		uint8_t out = 0, in = 0;
		void Delete(Sector);
		void inOutString(int inValue, String& in, int outValue, String& out);
	
	public:
		void InitializeGame() {}
	
		void SerializeConfigCustom(JsonObject&);
		void ProcessConfig(JsonObject&);
	
		void PreConfig();
		void GameConfig(Pair);
		
		bool PreCustom();
		bool CustomConfig(Pair);
		
		void CalculateScore(Sector);
		void Status();
		void Correct();
		
		X01() {
			gameID = "X01";
			name = "X01";
		}

		float GetAverageScore();
		void SerializeGameConfig(JsonObject&);
};

#endif
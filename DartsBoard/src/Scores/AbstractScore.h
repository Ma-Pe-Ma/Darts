#ifndef ABSTRACTSCORE_H
#define ABSTRACTSCORE_H

#include "../Hardware/DisplayContainer.h"
#include "../Hardware/Sector.h"
#include <ArduinoJson.h>

class AbstractScore {
	
	protected:
		
	public:
		int playerScore = 0;
		int position = -1;
		int roundCounter = 0;
		virtual float GetAverageScore() = 0;
		virtual void SerializePlayerStatus(JsonObject&) = 0;
		virtual void SerializeDartStatus(JsonObject body, Sector sector) = 0;

		virtual void DrawCompleteCustomStatus() {}
		virtual bool Score(Sector) {}
		virtual void Delete(Sector) {}
		virtual void Status();
		virtual void Correct(Sector&);
		virtual void StatusAfterHit(Sector) {}
};

#endif
#include "CricketScore.h"

int CricketScore::scoreMap[21];
int CricketScore::indicatorPositionMap[21];

CricketType CricketScore::cricketType = score;
CricketNumberSet CricketScore::cricketNumberSet = classicNumbers;
CricketCustomSet CricketScore::cricketCustomSet = interval;

CricketScore::CricketScore() {
	for (int i = 0; i < 21; i++) {
		cricketStatus[i] = 0;
	}
}

int CricketScore::GetIndicatorPosition(int base) {
	if (base == 25 || base == 0) {
		return 21;
	}
	
	return indicatorPositionMap[base];
}

int CricketScore::GetMappedScore(Sector sector) {
	if (sector.base == 25) {
		return 25;
	}
	
	return scoreMap[sector.base];
}

int CricketScore::GetStatusPosition(Sector sector) {
	if (sector.base == 25) {
		return 0;
	}
	
	return sector.base;
}

int CricketScore::GetSectorCloseState(int base) {
	if (base == 25) {
		base = 0;
	}
	
	int highestState = 0;
	
	for(int k = 0; k < Player::number; k++) {
		if(k != Player::cursor) {				
			
			int temp = ((CricketScore*) Player::players[k].score)->cricketStatus[base];
			
			if (temp > highestState) {
				highestState = temp;
			}
		}
	}
	
	return highestState;
}

bool CricketScore::Score(Sector sector) {
	int initStatus = cricketStatus[GetStatusPosition(sector)];
	
	cricketStatus[GetStatusPosition(sector)] += sector.multiplier;
	int newStatus = cricketStatus[GetStatusPosition(sector)];
	
	int sectorCloseStatus;
	
	switch (cricketType) {
		case score:
			
			sectorCloseStatus = GetSectorCloseState(sector.base);
			
			if (sectorCloseStatus < 3) {
				int diff = newStatus - initStatus;
				
				if (newStatus < 4) {
					diff = 0;
				}
				else if (initStatus < 4){
					diff = newStatus - 3;
				}
				
				playerScore += diff * GetMappedScore(sector);
			}			
			break;
		case noscore:
			playerScore = 0;
			break;
		case cutthroat: 
			//TODO:
			break;
	}
	
	int indicatorPosition;
	indicatorPosition = GetIndicatorPosition(sector.base);
			
	if (indicatorPosition != -1) {
		DrawSectorIndicator(GetIndicatorPosition(sector.base), sector.base, newStatus, sectorCloseStatus);
	}
	
	return CheckWinningCondition();
}

void CricketScore::Delete(Sector sector) {
	int initStatus = cricketStatus[GetStatusPosition(sector)];
	
	cricketStatus[GetStatusPosition(sector)] -= sector.multiplier;
	int newStatus = cricketStatus[GetStatusPosition(sector)];
	
	int sectorCloseStatus;
	
	switch (cricketType) {
		case score:
			
			sectorCloseStatus = GetSectorCloseState(sector.base);
			
			if (sectorCloseStatus < 3) {
				int diff = newStatus - initStatus;
				
				if (initStatus < 4) {
					diff = 0;
				}
				else if (newStatus < 4){
					diff = initStatus - 3;
				}
				
				playerScore -= diff * GetMappedScore(sector);
			}			
			break;
		case noscore:
			playerScore = 0;
			break;
		case cutthroat: 
		
			break;
	}
	
	int indicatorPosition;
	indicatorPosition = GetIndicatorPosition(sector.base);
			
	if (indicatorPosition != -1) {
		DrawSectorIndicator(GetIndicatorPosition(sector.base), sector.base, newStatus, sectorCloseStatus);
	}
}

bool CricketScore::CheckWinningCondition() {	
	//all sectors are closed
	bool condition1 = true;
	for (int i = 0; i < 21; i++) {
		if (scoreMap[i] != 0) {
			if (cricketStatus[i] < 3) {
				condition1 = false;
				break;
			}
		}		
	}
	
	//seperate conditions for different types
	bool condition2 = true;
	switch (cricketType) {
		case score:	
			for(int k = 0; k < Player::number; k++) {
				Player* checkable = &Player::players[k];

				if(k != Player::cursor && checkable->score->position == -1) {				
					if(playerScore < ((CricketScore*) checkable->score)->playerScore) {
						condition2 = false;
						break;
					}
				}			
			}	
			break;
		case noscore:
			condition2 = true;
			break;	
		case cutthroat:
		
			break;	
	}

	return (condition1 && condition2);
}

void CricketScore::DrawCompleteCustomStatus() {
	for (int i = 0; i < 21; i++) {
		int indicatorPosition = GetIndicatorPosition(i);
		
		if (indicatorPosition != -1) {
			int sectorCloseStatus = GetSectorCloseState(i);
			DrawSectorIndicator(indicatorPosition, i, cricketStatus[i], sectorCloseStatus);
		}		
	}
}

void CricketScore::StatusAfterHit(Sector dart) {
	int statusPosition = GetStatusPosition(dart);
	int indicatorPosition = GetIndicatorPosition(dart.base);
	
	if (indicatorPosition != -1) {
		int sectorCloseStatus = GetSectorCloseState(dart.base);
		DrawSectorIndicator(indicatorPosition, statusPosition, cricketStatus[statusPosition], sectorCloseStatus);
	}		
}

void CricketScore::DrawSectorIndicator(int position, int number, int innerState, int outerState) {
	if (number == 0) {
		number = 25;
	}
	
	int gridX = (position - 1) % 7;
	int gridY = (position - 1) / 7;
	
	int startX = int(SCR_WIDTH * 0.08f);
	int startY = int(SCR_HEIGHT * 0.634f);

	int offsetX = int(SCR_WIDTH * 0.14f);
	int offsetY = int(SCR_HEIGHT * 0.1375f);

	int centerX = startX + gridX * offsetX;
	int centerY = startY + gridY * offsetY;

	int halfRectWidth = int(SCR_WIDTH * 0.0625f);
	int halfRectHeigth = int(SCR_HEIGHT * 0.04167f);

	int outerStateLeft = CYAN;
	int outerStateRight = CYAN;

	//Drawing outer indicator
	if (outerState == 1 || outerState > 2) {
		outerStateLeft = BLUE;
	}
	
	if (outerState > 1) {
		outerStateRight = BLUE;
	}

	int border = int(SCR_WIDTH * 0.005f);

	DisplayContainer::displayContainer.getTFT()->fillRect(centerX - halfRectWidth - border, centerY - halfRectHeigth - border, halfRectWidth + border, 2 * (halfRectHeigth + border), outerStateLeft);
	DisplayContainer::displayContainer.getTFT()->fillRect(centerX, centerY - halfRectHeigth - border, halfRectWidth + border, 2 * (halfRectHeigth + border), outerStateRight);
	
	//Drawing inner indicator	
	int rightSide = innerState > 1 ? GREEN : CYAN;
	int leftSide = (innerState == 1 || innerState > 2) ? GREEN : CYAN;

	DisplayContainer::displayContainer.getTFT()->fillRect(centerX - halfRectWidth, centerY - halfRectHeigth, halfRectWidth, 2 * halfRectHeigth, leftSide);
	DisplayContainer::displayContainer.getTFT()->fillRect(centerX, centerY - halfRectHeigth, halfRectWidth, 2 * halfRectHeigth, rightSide);
	
	//Writing number to the center
	int textSize = 2;
	int pos = number < 10 ? (- textSize * 6) / 2 : (- 2 * textSize * 6) / 2;
	DisplayContainer::displayContainer.Write(centerX + pos, centerY - textSize * 6 / 2, RED, textSize, String(number));
}

void CricketScore::CreateScoreMap(CricketNumberSet set, CricketCustomSet customSet, int cricketNr, int cricketStart) {	
	for (int i = 1; i < 21; i++) {
		scoreMap[i] = 0;
		indicatorPositionMap[i] = -1;
	}
	
	scoreMap[0] = 25;
	
	if (set == classicNumbers) {
		cricketNr = 6;
		cricketStart = 15;	
	}

	if (set == allNumbers) {
		cricketNr = 20;
		cricketStart = 1;
	}
	
	if (set == customNumbers) {
		if (customSet == interval) {
			//
		}
		
		if (customSet == randomInterval) {
			cricketStart = random(1, 21 + 1 - cricketNr);
		}
	}
	
	if (set == customNumbers && customSet == chaotic) {		
		int* sector = new int[cricketNr];
		
		for (int i = 0; i < cricketNr; i++) {
			int newRandom = random(1,21);
			
			bool repeating = false;
			
			for (int j = 0; j < i; j++) {
				if (newRandom == sector[j]) {
					repeating = true;
					break;
				}
			}
			
			if (repeating) {
				i--;
				continue;
			}
			
			sector[i] = newRandom;
		}
		
		int value = 20;
		
		for (int i = 0; i < cricketNr; i++) {
			scoreMap[sector[i]] = value;
			indicatorPositionMap[sector[i]] = value;
			value--;
		}
		
		delete[] sector;
	}
	else {
		for (int i = cricketStart; i < cricketStart + cricketNr; i++) {
			scoreMap[i] = (21 - cricketNr) + (i - cricketStart);
			indicatorPositionMap[i] = (21 - cricketNr) + (i - cricketStart);
		}
	}
}

float CricketScore::GetAverageScore() {
	return 0;
}

void CricketScore::SerializePlayerStatus(JsonObject& body) {
	JsonObject states = body.createNestedObject("STATES");

	states["25"] = cricketStatus[0];

	for (int i = 1; i < 21; i++ ) {
		states[String(i)] = cricketStatus[i];
	}
}

void CricketScore::SerializeDartStatus(JsonObject body, Sector sector) {
	body[String(sector.base)] = cricketStatus[sector.base];
}
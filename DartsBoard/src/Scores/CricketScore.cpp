#include "CricketScore.h"

//#include "../Player.h"

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

int CricketScore::getIndicatorPosition(int base) {
	if (base == 25 || base == 0) {
		return 21;
	}
	
	return indicatorPositionMap[base];
}

int CricketScore::getMappedScore(Sector sector) {
	if (sector.base == 25) {
		return 25;
	}
	
	return scoreMap[sector.base];
}

int CricketScore::getStatusPosition(Sector sector) {
	if (sector.base == 25) {
		return 0;
	}
	
	return sector.base;
}

int CricketScore::getSectorCloseState(int base) {
	if (base == 25) {
		base = 0;
	}
	
	int highestState = 0;
	
	/*for(int k = 0; k < Player::getNumberOfPlayers(); k++) {
		if(Player::getCurrentPlayer() != Player::getPlayerByNumber(k)) {			
			int temp = ((CricketScore*) Player::getPlayerByNumber(k)->getScore())->cricketStatus[base];
			
			if (temp > highestState) {
				highestState = temp;
			}
		}
	}*/
	
	return highestState;
}

ThrowResult CricketScore::scoreThrow(Sector sector) {
	int initStatus = cricketStatus[getStatusPosition(sector)];
	
	cricketStatus[getStatusPosition(sector)] += sector.multiplier;
	int newStatus = cricketStatus[getStatusPosition(sector)];
	
	int sectorCloseStatus;
	
	switch (cricketType) {
		case score:			
			sectorCloseStatus = getSectorCloseState(sector.base);
			
			if (sectorCloseStatus < 3) {
				int diff = newStatus - initStatus;
				
				if (newStatus < 4) {
					diff = 0;
				}
				else if (initStatus < 4){
					diff = newStatus - 3;
				}
				
				playerScore += diff * getMappedScore(sector);
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
	indicatorPosition = getIndicatorPosition(sector.base);
			
	if (indicatorPosition != -1) {
		drawSectorIndicator(getIndicatorPosition(sector.base), sector.base, newStatus, sectorCloseStatus);
	}
	
	return checkWinningCondition();
}

void CricketScore::deleteThrow(Sector sector) {
	int initStatus = cricketStatus[getStatusPosition(sector)];
	
	cricketStatus[getStatusPosition(sector)] -= sector.multiplier;
	int newStatus = cricketStatus[getStatusPosition(sector)];
	
	int sectorCloseStatus;
	
	switch (cricketType) {
		case score:			
			sectorCloseStatus = getSectorCloseState(sector.base);
			
			if (sectorCloseStatus < 3) {
				int diff = newStatus - initStatus;
				
				if (initStatus < 4) {
					diff = 0;
				}
				else if (newStatus < 4){
					diff = initStatus - 3;
				}
				
				playerScore -= diff * getMappedScore(sector);
			}			
			break;
		case noscore:
			playerScore = 0;
			break;
		case cutthroat: 
		
			break;
	}
	
	int indicatorPosition;
	indicatorPosition = getIndicatorPosition(sector.base);
			
	if (indicatorPosition != -1) {
		drawSectorIndicator(getIndicatorPosition(sector.base), sector.base, newStatus, sectorCloseStatus);
	}
}

ThrowResult CricketScore::checkWinningCondition() {	
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
			/*for(int k = 0; k < Player::getNumberOfPlayers(); k++) {
				Player* checkable = Player::getPlayerByNumber(k);

				if(checkable != Player::getCurrentPlayer() && checkable->getScore()->getPosition() == -1) {				
					if(playerScore < ((CricketScore*) checkable->getScore())->playerScore) {
						condition2 = false;
						break;
					}
				}			
			}*/	
			break;
		case noscore:
			condition2 = true;
			break;	
		case cutthroat:
		
			break;	
	}

	ThrowResult throwResult;

	if (condition1 && condition2) {
		throwResult.throwType = ThrowType::winning;
	}
	else {
		throwResult.throwType = ThrowType::normal;
	}

	return throwResult;
}

void CricketScore::drawCompleteCustomStatus() {
	for (int i = 0; i < 21; i++) {
		int indicatorPosition = getIndicatorPosition(i);
		
		if (indicatorPosition != -1) {
			int sectorCloseStatus = getSectorCloseState(i);
			drawSectorIndicator(indicatorPosition, i, cricketStatus[i], sectorCloseStatus);
		}		
	}
}

void CricketScore::statusAfterHit(Sector dart) {
	int statusPosition = getStatusPosition(dart);
	int indicatorPosition = getIndicatorPosition(dart.base);
	
	if (indicatorPosition != -1) {
		int sectorCloseStatus = getSectorCloseState(dart.base);
		drawSectorIndicator(indicatorPosition, statusPosition, cricketStatus[statusPosition], sectorCloseStatus);
	}		
}

void CricketScore::drawSectorIndicator(int position, int number, int innerState, int outerState) {
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

	/*DisplayContainer::displayContainer.getTFT()->fillRect(centerX - halfRectWidth - border, centerY - halfRectHeigth - border, halfRectWidth + border, 2 * (halfRectHeigth + border), outerStateLeft);
	DisplayContainer::displayContainer.getTFT()->fillRect(centerX, centerY - halfRectHeigth - border, halfRectWidth + border, 2 * (halfRectHeigth + border), outerStateRight);
	
	//Drawing inner indicator	
	int rightSide = innerState > 1 ? GREEN : CYAN;
	int leftSide = (innerState == 1 || innerState > 2) ? GREEN : CYAN;

	DisplayContainer::displayContainer.getTFT()->fillRect(centerX - halfRectWidth, centerY - halfRectHeigth, halfRectWidth, 2 * halfRectHeigth, leftSide);
	DisplayContainer::displayContainer.getTFT()->fillRect(centerX, centerY - halfRectHeigth, halfRectWidth, 2 * halfRectHeigth, rightSide);
	
	//Writing number to the center
	int textSize = 2;
	int pos = number < 10 ? (- textSize * 6) / 2 : (- 2 * textSize * 6) / 2;
	DisplayContainer::displayContainer.write(centerX + pos, centerY - textSize * 6 / 2, RED, textSize, String(number));*/
}

void CricketScore::createScoreMap(CricketNumberSet set, CricketCustomSet customSet, int cricketNr, int cricketStart) {	
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

float CricketScore::getAverageScore() {
	return 0;
}

void CricketScore::serializePlayerStatus(JsonObject& body) {
	JsonObject states = body.createNestedObject("STATES");

	states["25"] = cricketStatus[0];

	for (int i = 1; i < 21; i++ ) {
		states[String(i)] = cricketStatus[i];
	}
}

void CricketScore::serializeDartStatus(JsonObject body, Sector sector) {
	body[String(sector.base)] = cricketStatus[sector.base];
}
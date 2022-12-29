#include "CricketScore.h"

#include "../Player/PlayerContainer.h"

CricketScore::CricketScore(DisplayContainer* displayContainer, PlayerContainer* playerContainer) : AbstractScore(displayContainer, playerContainer) {
	for (int i = 0; i < 21; i++) {
		cricketStatus[i] = 0;
	}
}

//method telling 
int CricketScore::getIndicatorPosition(Sector sector) {
	if (sector.base == 25 || sector.base == 0 ) {
		return 21;
	}
	
	return scoreMap[sector.base];
}

int CricketScore::getMappedScore(Sector sector) {
	if (sector.base == 25) {
		return 25;
	}
	
	return scoreMap[sector.base];
}

//only needed because bullseye
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
	
	for(int k = 0; k < playerContainer->getNumberOfPlayers(); k++) {
		if(playerContainer->getCurrentPlayer() != playerContainer->getPlayerByNumber(k)) {			
			int temp = ((CricketScore*) playerContainer->getPlayerByNumber(k)->getScore())->cricketStatus[base];
			
			if (temp > highestState) {
				highestState = temp;
			}
		}
	}
	
	return highestState;
}

ThrowResult CricketScore::scoreThrow(Sector sector) {
	int mappedScore = getMappedScore(sector);
	if (mappedScore == -1) {
		ThrowResult throwResult;
		throwResult.throwType = ThrowType::missed;
		return throwResult;
	}

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
				
				playerScore += diff * mappedScore;
			}			
			break;
		case noscore:
			playerScore = 0;
			break;
		case cutthroat: 
			//TODO:
			break;
	}

	//drawSectorIndicator(getIndicatorPosition(sector), sector.base, newStatus, sectorCloseStatus);
	return checkWinningCondition();
}

void CricketScore::deleteThrow(Sector sector) {
	int mappedScore = getMappedScore(sector);
	if (mappedScore == -1) {
		return;
	}

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

	drawSectorIndicator(getIndicatorPosition(sector), sector.base, newStatus, sectorCloseStatus);
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
			for(int k = 0; k < playerContainer->getNumberOfPlayers(); k++) {
				Player* checkable = playerContainer->getPlayerByNumber(k);

				if (checkable != playerContainer->getCurrentPlayer() && checkable->getScore()->getPosition() == -1) {				
					if (playerScore < ((CricketScore*) checkable->getScore())->playerScore) {
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
		Sector sector = {.base = i, .multiplier = 0};
		int mappedScore = getMappedScore(sector);

		if (mappedScore == -1) {
			continue;
		}

		int indicatorPosition = getIndicatorPosition(sector);		
		int sectorCloseStatus = getSectorCloseState(i);

		drawSectorIndicator(indicatorPosition, i, cricketStatus[i], sectorCloseStatus);		
	}
}

void CricketScore::statusAfterHit(Sector dart) {
	int mappedScore = getMappedScore(dart);

	if (mappedScore == -1) {
		return;
	}

	int statusPosition = getStatusPosition(dart);
	int indicatorPosition = getIndicatorPosition(dart);
	int sectorCloseStatus = getSectorCloseState(dart.base);
	drawSectorIndicator(indicatorPosition, statusPosition, cricketStatus[statusPosition], sectorCloseStatus);		
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

	int outerStateLeft =TFT_CYAN;
	int outerStateRight =TFT_CYAN;

	//Drawing outer indicator
	if (outerState == 1 || outerState > 2) {
		outerStateLeft = TFT_BLUE;
	}
	
	if (outerState > 1) {
		outerStateRight = TFT_BLUE;
	}

	int border = int(SCR_WIDTH * 0.005f);

	displayContainer->getTFT()->fillRect(centerX - halfRectWidth - border, centerY - halfRectHeigth - border, halfRectWidth + border, 2 * (halfRectHeigth + border), outerStateLeft);
	displayContainer->getTFT()->fillRect(centerX, centerY - halfRectHeigth - border, halfRectWidth + border, 2 * (halfRectHeigth + border), outerStateRight);
	
	//Drawing inner indicator	
	int rightSide = innerState > 1 ? TFT_GREEN : TFT_CYAN;
	int leftSide = (innerState == 1 || innerState > 2) ? TFT_GREEN : TFT_CYAN;

	displayContainer->getTFT()->fillRect(centerX - halfRectWidth, centerY - halfRectHeigth, halfRectWidth, 2 * halfRectHeigth, leftSide);
	displayContainer->getTFT()->fillRect(centerX, centerY - halfRectHeigth, halfRectWidth, 2 * halfRectHeigth, rightSide);
	
	//Writing number to the center
	int textSize = 2;
	int pos = number < 10 ? (- textSize * 6) / 2 : (- 2 * textSize * 6) / 2;
	displayContainer->write(centerX + pos, centerY - textSize * 6 / 2, TFT_RED, textSize, String(number).c_str());
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

void CricketScore::setCricketMap(int* scoreMap) {
	for (int i = 0; i < 21; i ++) {
		this->scoreMap[i] = scoreMap[i];
	}
}

void CricketScore::setGameType(CricketType cricketType, CricketNumberSet cricketNumberSet, CricketCustomSet cricketCustomSet) {
	this->cricketType = cricketType;
	this->cricketNumberSet = cricketNumberSet;
	this->cricketCustomSet = cricketCustomSet;
}
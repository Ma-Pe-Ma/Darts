#include "Cricket.h"

void Cricket::initialize(DisplayContainer* displayContainer, PlayerContainer* playerContainer) {
	this->displayContainer = displayContainer;
	this->playerContainer = playerContainer;
	
	this->gameID = "CRICKET";
	this->name = "Cricket";
	
	this->initializeMaps();

	/*scoreString = Resources::getTextByID(Resources::Text::cricketScore);
	noscoreString = Resources::getTextByID(Resources::Text::cricketNoscore);
	cutthroatString = Resources::getTextByID(Resources::Text::cricketCutthroat);

	classicString = Resources::getTextByID(Resources::Text::cricketClassic);
	allString = Resources::getTextByID(Resources::Text::cricketAll);
	customString = Resources::getTextByID(Resources::Text::cricketCustom);

	intervalString = Resources::getTextByID(Resources::Text::cricketInterval);
	randintervalString = Resources::getTextByID(Resources::Text::cricketRandinterval);
	chaoticString = Resources::getTextByID(Resources::Text::cricketChaotic);

	nrOfNrsString = Resources::getTextByID(Resources::Text::cricketNrOfNrs);
	startingNrString = Resources::getTextByID(Resources::Text::cricketStartingNr);*/
}

void Cricket::initializeMaps() {
	typeMap.insert(CricketType::score, String("SCORE"));
	typeMap.insert(CricketType::noscore, String("NOSCORE"));
	typeMap.insert(CricketType::cutthroat, String("CUTTHROAT"));

	setMap.insert(CricketNumberSet::classicNumbers, String("CLASSIC"));
	setMap.insert(CricketNumberSet::allNumbers, String("ALL"));
	setMap.insert(CricketNumberSet::customNumbers, String("CUSTOM"));

	customMap.insert(CricketCustomSet::interval, String("INTERVAL"));
	customMap.insert(CricketCustomSet::randomInterval, String("RINTERVAL"));
	customMap.insert(CricketCustomSet::chaotic, String("CHAOTIC"));
}

void Cricket::processConfig(JsonObject& message) {
	cricketNr = message["CricketNr"].as<int>();
	cricketType = typeMap.getKeyByValue(message["CricketType"]);
	cricketNumberSet = setMap.getKeyByValue(message["CricketNumberSet"]);
	cricketCustomSet = customMap.getKeyByValue(message["CricketCustomSet"]);
	cricketStart = message["CricketStart"];
}

void Cricket::processGamePlay(JsonObject message) {
	
}

void Cricket::drawProperButtons(int index, int id) {
	if (index == 0) {
		scoreButton.guiButton.drawButton(id != 1);
		noscoreButton.guiButton.drawButton(id != 2);
		cutthroatButton.guiButton.drawButton(id != 3);
	}
	else if (index == 1) {
		classicButton.guiButton.drawButton(id != 1);
		allButton.guiButton.drawButton(id != 2);
		customButton.guiButton.drawButton(id != 3);
	}
}

void Cricket::configStart() {
	int buttonWidth = int(SCR_WIDTH * 0.35f);
	int buttonHeight = int(SCR_HEIGHT * 0.166f);	
	
	int buttonStartX = int(SCR_WIDTH * 0.2f);
	int buttonStartY = int(SCR_HEIGHT * 0.375f);

	int buttonOffsetX = int(SCR_WIDTH * 0.6f);
	int buttonOffsetY = int(SCR_HEIGHT * 0.25f);

	scoreButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 0 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) scoreString.c_str(), 2);
	noscoreButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 1 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) noscoreString.c_str(), 2);
	cutthroatButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 2 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) cutthroatString.c_str(), 2);

	switch (cricketType) {
		case score:
			drawProperButtons(0, 1);
			break;
		
		case noscore:
			drawProperButtons(0, 2);
			break;
		
		case cutthroat:
			drawProperButtons(0, 3);
			break;
	}
	
	classicButton.setImage(displayContainer->getTFT(), buttonStartX + buttonOffsetX, buttonStartY + 0 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) classicString.c_str(), 2);
	allButton.setImage(displayContainer->getTFT(), buttonStartX + buttonOffsetX, buttonStartY + 1 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) allString.c_str(), 2);
	customButton.setImage(displayContainer->getTFT(), buttonStartX + buttonOffsetX, buttonStartY + 2 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) customString.c_str(), 2);
	
	switch (cricketNumberSet) {
		case classicNumbers:
			drawProperButtons(1, 1);
			break;
			
		case allNumbers:
			drawProperButtons(1, 2);
			break;
		
		case customNumbers:
			drawProperButtons(1, 3);
			break;
	}
}

void Cricket::config(Pair touch) {
	scoreButton.detect(touch);
	noscoreButton.detect(touch);
	cutthroatButton.detect(touch);
	
	if (scoreButton.simple()) {
		cricketType = score;
		drawProperButtons(0, 1);
		serializeConfig();
	}
	
	if (noscoreButton.simple()) {
		cricketType = noscore;
		drawProperButtons(0, 2);
		serializeConfig();
	}
	
	if (cutthroatButton.simple()) {
		cricketType = cutthroat;
		drawProperButtons(0, 3);
		serializeConfig();
	}
	
	classicButton.detect(touch);
	allButton.detect(touch);
	customButton.detect(touch);
	
	if (classicButton.simple()) {
		cricketNumberSet = classicNumbers;
		drawProperButtons(1, 1);
		serializeConfig();
	}
	
	if (allButton.simple()) {
		cricketNumberSet = allNumbers;
		drawProperButtons(1, 2);
		serializeConfig();
	}
	
	if (customButton.simple()) {
		cricketNumberSet = customNumbers;
		drawProperButtons(1, 3);
		serializeConfig();
	}
}

bool Cricket::customStart() {
	if (cricketNumberSet != customNumbers) {
		return false;
	}

	int buttonWidth = int(SCR_WIDTH * 0.35f);
	int buttonHeight = int(SCR_HEIGHT * 0.166f);	

	int buttonStartX = int(SCR_WIDTH * 0.2f);
	int buttonStartY = int(SCR_HEIGHT * 0.375f);
	
	int buttonOffsetX = int(SCR_WIDTH * 0.6f);
	int buttonOffsetY = int(SCR_HEIGHT * 0.25f);
	
	//setting up 
	intervalButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 0 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) intervalString.c_str(), 2);
	randomIntervalButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 1 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) randintervalString.c_str(), 2);
	chaoticButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 2 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) chaoticString.c_str(), 2);
	
	switch (cricketCustomSet) {
		case interval:
			intervalButton.guiButton.drawButton(false);
			randomIntervalButton.guiButton.drawButton(true);
			chaoticButton.guiButton.drawButton(true);
			break;
		
		case randomInterval:
			intervalButton.guiButton.drawButton(true);
			randomIntervalButton.guiButton.drawButton(false);
			chaoticButton.guiButton.drawButton(true);
			break;
		
		case chaotic:
			intervalButton.guiButton.drawButton(true);
			randomIntervalButton.guiButton.drawButton(true);
			chaoticButton.guiButton.drawButton(false);
			break;
	}
	
	int textSize = 2;
	int buttonSize = int(SCR_WIDTH * 0.1f);
	int nrButtonY = SCR_HEIGHT / 2;

	int textStartX = int(SCR_WIDTH * 0.6375f);
	int textStartY = nrButtonY - buttonSize - int(textSize * 6);

	//Draw number of numbers selector + buttons
	displayContainer->write(textStartX, textStartY, RED, textSize, nrOfNrsString.c_str());
	drawCricketNr();

	int nrButtonX = int(SCR_WIDTH * 0.7f);
	int nrButtonOffsetX = int(SCR_WIDTH * 0.2f);
	
	prevNr.setImage(displayContainer->getTFT(), nrButtonX, nrButtonY, buttonSize, buttonSize, WHITE, GREEN, BLACK, "<", 2);
	nextNr.setImage(displayContainer->getTFT(), nrButtonX + nrButtonOffsetX, nrButtonY, buttonSize, buttonSize, WHITE, GREEN, BLACK, ">", 2);
	prevNr.guiButton.drawButton(true);
	nextNr.guiButton.drawButton(true);
	
	//draw starting number + buttons if interval is chosen
	if (cricketCustomSet == interval) {
		int startButtonY = int(SCR_HEIGHT * 0.875f);
		
		textStartY = startButtonY - buttonSize - int(textSize * 6);
		displayContainer->write(textStartX, textStartY, RED, textSize, startingNrString.c_str());
		this->drawCricketStart();
		
		prevStart.setImage(displayContainer->getTFT(), nrButtonX, startButtonY, buttonSize, buttonSize, WHITE, GREEN, BLACK, "<", 2);
		nextStart.setImage(displayContainer->getTFT(), nrButtonX + nrButtonOffsetX, startButtonY, buttonSize, buttonSize, WHITE, GREEN, BLACK, ">", 2);

		prevStart.guiButton.drawButton(true);
		nextStart.guiButton.drawButton(true);		
	}
	
	return true;
}

bool Cricket::custom(Pair touch) {
	intervalButton.detect(touch);
	randomIntervalButton.detect(touch);
	chaoticButton.detect(touch);
	
	if (intervalButton.simple()) {
		cricketCustomSet = interval;
		intervalButton.guiButton.drawButton(false);
		randomIntervalButton.guiButton.drawButton(true);
		chaoticButton.guiButton.drawButton(true);

		prevStart.guiButton.drawButton(true);
		nextStart.guiButton.drawButton(true);
		drawCricketStart();

		displayContainer->write(textStartX, textStartY, RED, 2, (char*) startingNrString.c_str());
		serializeConfig();
	}
	
	if (randomIntervalButton.simple()) {
		cricketCustomSet = randomInterval;
		intervalButton.guiButton.drawButton(true);
		randomIntervalButton.guiButton.drawButton(false);
		chaoticButton.guiButton.drawButton(true);

		int textLength = 12;
		displayContainer->getTFT()->fillRect(textStartX, textStartY, textLength * textSize * 6 , rectSize, YELLOW);
		serializeConfig();
	}
	
	if (chaoticButton.simple()) {
		cricketCustomSet = chaotic;
		intervalButton.guiButton.drawButton(true);
		randomIntervalButton.guiButton.drawButton(true);
		chaoticButton.guiButton.drawButton(false);

		int textLength = 12;
		displayContainer->getTFT()->fillRect(textStartX, textStartY, textLength * textSize * 6 , rectSize, YELLOW);
		serializeConfig();
	}
	
	prevNr.detect(touch);
	nextNr.detect(touch);
	
	if (prevNr.simple()) {
		prevNr.guiButton.drawButton(false);
		
		if (--cricketNr < 6) {
			cricketNr = 6;
		}
		
		drawCricketNr();		
		serializeConfig();
	}
	
	if (prevNr.released()) {
		prevNr.guiButton.drawButton(true);
	}
	
	if (nextNr.simple()) {
		nextNr.guiButton.drawButton(false);
		
		if (++cricketNr > 20) {
			cricketNr = 20;
		}
		
		drawCricketNr();
	
		if (cricketStart > 21 - cricketNr) {
			cricketStart = 21 - cricketNr;
			if (cricketCustomSet == interval) {
				drawCricketStart();
			}
		}
		
		serializeConfig();
	}
	
	if (nextNr.released()) {
		nextNr.guiButton.drawButton(true);
	}
	
	if (cricketCustomSet == interval) {
		prevStart.detect(touch);
		nextStart.detect(touch);
		
		if (prevStart.simple()) {
			prevStart.guiButton.drawButton(false);
			
			if(--cricketStart < 1) {
				cricketStart = 1;
			}
			
			drawCricketStart();
			serializeConfig();
		}
		
		if (prevStart.released()) {
			prevStart.guiButton.drawButton(true);
		}
		
		if (nextStart.simple()) {
			nextStart.guiButton.drawButton(false);
			
			if(++cricketStart > 21 - cricketNr) {
				cricketStart = 21 - cricketNr;
			}
			
			drawCricketStart();
			serializeConfig();
		}
		
		if (nextStart.released()) {
			nextStart.guiButton.drawButton(true);
		}
	}

	return false;
}

void Cricket::drawCricketNr() {
	int startX = int(SCR_WIDTH * 0.775f);
	int startY = int(SCR_HEIGHT * 0.48f);

	displayContainer->writeWithBackground(startX, startY, RED, YELLOW, 2, cricketNr > 9 ? String(cricketNr) : " " + String(cricketNr));
}

void Cricket::drawCricketStart() {
	int startX = int(SCR_WIDTH * 0.775f);
	int startY = int(SCR_HEIGHT * 0.85f);

	displayContainer->writeWithBackground(startX, startY, RED, YELLOW, 2, cricketStart > 9 ? String(cricketStart) : " " +String(cricketStart));
}

void Cricket::initializeGame() {
	for (int i = 1; i < 21; i++) {
		scoreMap[i] = -1;
	}
	
	scoreMap[0] = 25;
	
	if (cricketNumberSet == classicNumbers) {
		cricketNr = 6;
		cricketStart = 15;	
	}
	else if (cricketNumberSet == allNumbers) {
		cricketNr = 20;
		cricketStart = 1;
	}
	else if (cricketNumberSet == customNumbers) {
		if (cricketCustomSet == interval) {
			//do not modify arguments
		}
		
		if (cricketCustomSet == randomInterval) {
			cricketStart = random(1, 21 + 1 - cricketNr);
		}
	}
	
	if (cricketNumberSet == customNumbers && cricketCustomSet == chaotic) {		
		int* sector = new int[cricketNr];
		
		for (int i = 0; i < cricketNr; i++) {
			int newRandom = random(1, 21);
			
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
			value--;
		}
		
		delete[] sector;
	}
	else {
		for (int i = cricketStart; i < cricketStart + cricketNr; i++) {
			scoreMap[i] = (21 - cricketNr) + (i - cricketStart);
		}
	}

	//for (int i = 0; i < 21; i++) {
	//	Serial.println(String(i) + ".th - " + String(scoreMap[i]));
	//}
}

void Cricket::serializeConfigCustom(JsonObject& configObject) {
	configObject["CricketNr"] = cricketNr;	
	configObject["CricketType"] = typeMap.getValueByKey(cricketType);
	configObject["CricketNumberSet"] = setMap.getValueByKey(cricketNumberSet);
	configObject["CricketCustomSet"] = customMap.getValueByKey(cricketCustomSet);
	configObject["CricketStart"] = cricketStart;

	JsonObject mapObject = configObject.createNestedObject("MAP");

	mapObject["0"] = 25;

	for (int i = 1; i < 21; i++) {
		mapObject[String(i)] = scoreMap[i];
	}
}
#include "Cricket.h"

SimpleMap<CricketType, String>* Cricket::typeMap;
SimpleMap<CricketNumberSet, String>* Cricket::setMap;
SimpleMap<CricketCustomSet, String>* Cricket::customMap;

Cricket::Cricket() {
	gameID = "CRICKET";
	name = "Cricket";
	InitializeMaps();
}

void Cricket::InitializeMaps() {
	typeMap = new SimpleMap<CricketType, String>(3);
	typeMap->Insert(score, String("SCORE"));
	typeMap->Insert(noscore, String("NOSCORE"));
	typeMap->Insert(cutthroat, String("CUTTHROAT"));
	
	setMap = new SimpleMap<CricketNumberSet, String>(3);
	setMap->Insert(classicNumbers, String("CLASSIC"));
	setMap->Insert(allNumbers, String("ALL"));
	setMap->Insert(customNumbers, String("CUSTOM"));
	
	customMap = new SimpleMap<CricketCustomSet, String>(3);
	customMap->Insert(interval, String("INTERVAL"));
	customMap->Insert(randomInterval, String("RANDOMINTERVAL"));
	customMap->Insert(chaotic, String("CHAOTIC"));
}

void Cricket::ProcessConfig(JsonObject& message) {
	cricketNr = message["CricketNr"].as<int>();
	cricketType = typeMap->GetKeyByValue(message["CricketType"]);
	cricketNumberSet = setMap->GetKeyByValue(message["CricketNumberSet"]);
	cricketCustomSet = customMap->GetKeyByValue(message["CricketCustomSet"]);
	cricketStart = message["CricketStart"];
}

void Cricket::ProcessGamePlay(JsonObject message) {
	
}

void Cricket::PreConfig() {
	scoreButton.setImage(DisplayContainer::displayContainer.getTFT(), 80, 90, 140, 40, WHITE, GREEN, BLACK, "Score", 2);
	noscoreButton.setImage(DisplayContainer::displayContainer.getTFT(), 80, 150, 140, 40, WHITE, GREEN, BLACK, "No-Score", 2);
	cutthroatButton.setImage(DisplayContainer::displayContainer.getTFT(), 80, 210, 140, 40, WHITE, GREEN, BLACK, "Cut-Thr.", 2);
	
	switch (cricketType) {
		case score:
			scoreButton.guiButton.drawButton(false);
			noscoreButton.guiButton.drawButton(true);
			cutthroatButton.guiButton.drawButton(true);
			break;
		
		case noscore:
			scoreButton.guiButton.drawButton(true);
			noscoreButton.guiButton.drawButton(false);
			cutthroatButton.guiButton.drawButton(true);
			break;
		
		case cutthroat:
			scoreButton.guiButton.drawButton(true);
			noscoreButton.guiButton.drawButton(true);
			cutthroatButton.guiButton.drawButton(false);
			break;
	}
	
	classicButton.setImage(DisplayContainer::displayContainer.getTFT(), 320, 90, 140, 40, WHITE, GREEN, BLACK, "Classic", 2);
	allButton.setImage(DisplayContainer::displayContainer.getTFT(), 320, 150, 140, 40, WHITE, GREEN, BLACK, "Every", 2);
	customButton.setImage(DisplayContainer::displayContainer.getTFT(), 320, 210, 140, 40, WHITE, GREEN, BLACK, "Custom", 2);
	
	switch (cricketNumberSet) {
		case classicNumbers:
			classicButton.guiButton.drawButton(false);
			allButton.guiButton.drawButton(true);
			customButton.guiButton.drawButton(true);
			break;
			
		case allNumbers:
			classicButton.guiButton.drawButton(true);
			allButton.guiButton.drawButton(false);
			customButton.guiButton.drawButton(true);
			break;
		
		case customNumbers:
			classicButton.guiButton.drawButton(true);
			allButton.guiButton.drawButton(true);
			customButton.guiButton.drawButton(false);
			break;
	}
}

void Cricket::GameConfig(Pair touch) {
	scoreButton.detect(touch);
	noscoreButton.detect(touch);
	cutthroatButton.detect(touch);
	
	if (scoreButton.simple()) {
		cricketType = score;
		scoreButton.guiButton.drawButton(false);
		noscoreButton.guiButton.drawButton(true);
		cutthroatButton.guiButton.drawButton(true);
		SerializeConfig();
	}
	
	if (noscoreButton.simple()) {
		cricketType = noscore;
		scoreButton.guiButton.drawButton(true);
		noscoreButton.guiButton.drawButton(false);
		cutthroatButton.guiButton.drawButton(true);
		SerializeConfig();
	}
	
	if (cutthroatButton.simple()) {
		cricketType = cutthroat;
		scoreButton.guiButton.drawButton(true);
		noscoreButton.guiButton.drawButton(true);
		cutthroatButton.guiButton.drawButton(false);
		SerializeConfig();
	}
	
	classicButton.detect(touch);
	allButton.detect(touch);
	customButton.detect(touch);
	
	if (classicButton.simple()) {
		cricketNumberSet = classicNumbers;
		classicButton.guiButton.drawButton(false);
		allButton.guiButton.drawButton(true);
		customButton.guiButton.drawButton(true);
		SerializeConfig();
	}
	
	if (allButton.simple()) {
		cricketNumberSet = allNumbers;
		classicButton.guiButton.drawButton(true);
		allButton.guiButton.drawButton(false);
		customButton.guiButton.drawButton(true);
		SerializeConfig();
	}
	
	if (customButton.simple()) {
		cricketNumberSet = customNumbers;
		classicButton.guiButton.drawButton(true);
		allButton.guiButton.drawButton(true);
		customButton.guiButton.drawButton(false);
		SerializeConfig();
	}
}

bool Cricket::PreCustom() {
	if (cricketNumberSet != customNumbers) {
		return false;
	}
	
	intervalButton.setImage(DisplayContainer::displayContainer.getTFT(), 80, 90, 140, 40, WHITE, GREEN, BLACK, "Interval", 2);
	randomIntervalButton.setImage(DisplayContainer::displayContainer.getTFT(), 80, 150, 140, 40, WHITE, GREEN, BLACK, "RandInt.", 2);
	chaoticButton.setImage(DisplayContainer::displayContainer.getTFT(), 80, 210, 140, 40, WHITE, GREEN, BLACK, "Chaotic", 2);
	
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
	
	prevNr.setImage(DisplayContainer::displayContainer.getTFT(), 280, 120, 40, 40, WHITE, GREEN, BLACK, "<", 2);
	nextNr.setImage(DisplayContainer::displayContainer.getTFT(), 360, 120, 40, 40, WHITE, GREEN, BLACK, ">", 2);
	prevNr.guiButton.drawButton(true);
	nextNr.guiButton.drawButton(true);
	DrawCricketNr();
	DisplayContainer::displayContainer.Write(255, 80, RED, 2, "Nr. of Nrs.:");
	
	prevStart.setImage(DisplayContainer::displayContainer.getTFT(), 280, 210, 40, 40, WHITE, GREEN, BLACK, "<", 2);
	nextStart.setImage(DisplayContainer::displayContainer.getTFT(), 360, 210, 40, 40, WHITE, GREEN, BLACK, ">", 2);
	
	if (cricketCustomSet == interval) {
		prevStart.guiButton.drawButton(true);
		nextStart.guiButton.drawButton(true);
		DrawCricketStart();
		DisplayContainer::displayContainer.Write(255, 165, RED, 2, "Starting nr:");
	}
	
	return true;
}

void Cricket::DrawCricketNr() {
	DisplayContainer::displayContainer.WriteWithBackground(310, 115, RED, YELLOW, 2, cricketNr > 9 ? String(cricketNr) : " " +String(cricketNr));
}

void Cricket::DrawCricketStart() {
	DisplayContainer::displayContainer.WriteWithBackground(310, 205, RED, YELLOW, 2, cricketStart > 9 ? String(cricketStart) : " " +String(cricketStart));
}

bool Cricket::CustomConfig(Pair touch) {
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
		DrawCricketStart();
		DisplayContainer::displayContainer.Write(255, 165, RED, 2, "Starting nr:");
		SerializeConfig();
	}
	
	if (randomIntervalButton.simple()) {
		cricketCustomSet = randomInterval;
		intervalButton.guiButton.drawButton(true);
		randomIntervalButton.guiButton.drawButton(false);
		chaoticButton.guiButton.drawButton(true);
		DisplayContainer::displayContainer.getTFT()->fillRect(255, 165, 140, 65, YELLOW);
		SerializeConfig();
	}
	
	if (chaoticButton.simple()) {
		cricketCustomSet = chaotic;
		intervalButton.guiButton.drawButton(true);
		randomIntervalButton.guiButton.drawButton(true);
		chaoticButton.guiButton.drawButton(false);
		DisplayContainer::displayContainer.getTFT()->fillRect(255, 165, 140, 65, YELLOW);
		SerializeConfig();
	}
	
	prevNr.detect(touch);
	nextNr.detect(touch);
	
	if (prevNr.simple()) {
		prevNr.guiButton.drawButton(false);
		
		if (--cricketNr < 6) {
			cricketNr = 6;
		}
		
		DrawCricketNr();
		
		SerializeConfig();
	}
	
	if (prevNr.released()) {
		prevNr.guiButton.drawButton(true);
	}
	
	if (nextNr.simple()) {
		nextNr.guiButton.drawButton(false);
		
		if (++cricketNr > 20) {
			cricketNr = 20;
		}
		
		DrawCricketNr();
	
		if (cricketStart > 21 - cricketNr) {
			cricketStart = 21 - cricketNr;
			if (cricketCustomSet == interval) {
				DrawCricketStart();
			}
		}
		
		SerializeConfig();
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
			
			DrawCricketStart();
			SerializeConfig();
		}
		
		if (prevStart.released()) {
			prevStart.guiButton.drawButton(true);
		}
		
		if (nextStart.simple()) {
			nextStart.guiButton.drawButton(false);
			
			if(++cricketStart > 21 - cricketNr) {
				cricketStart = 21 - cricketNr;
			}
			
			DrawCricketStart();
			SerializeConfig();
		}
		
		if (nextStart.released()) {
			nextStart.guiButton.drawButton(true);
		}
	}

	return false;
}

void Cricket::InitializeGame() {
	CricketScore::CreateScoreMap(cricketNumberSet, cricketCustomSet, cricketNr, cricketStart);	
}

void Cricket::InitializeRound() {
	
}

void Cricket::SerializeConfigCustom(JsonObject& configObject) {
	configObject["CricketNr"] = cricketNr;	
	configObject["CricketType"] = typeMap->GetValueByKey(cricketType);
	configObject["CricketNumberSet"] = setMap->GetValueByKey(cricketNumberSet);
	configObject["CricketCustomSet"] = customMap->GetValueByKey(cricketCustomSet);
	configObject["CricketStart"] = cricketStart;

	JsonObject mapObject = configObject.createNestedObject("MAP");

	mapObject["0"] = 25;

	for (int i = 1; i < 21; i++) {
		mapObject[String(i)] = CricketScore::scoreMap[i];
	}
}
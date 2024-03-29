#include "RoundTheClock.h"

#include <MCUFRIEND_kbv.h>

void RoundTheClock::initialize(DisplayContainer* displayContainer, PlayerContainer* playerContainer) {
	this->displayContainer = displayContainer;
	this->playerContainer = playerContainer;

	gameID = "RTC";
	name = "Round The Clock";

	sectorOnlyString = Resources::getTextByID(Resources::Text::rtcSector);
	multiplierString = Resources::getTextByID(Resources::Text::rtcMultiplier);
	multiplierWithPointsString = Resources::getTextByID(Resources::Text::rtcMultiplierWithPoints);

	rtcClassicString = Resources::getTextByID(Resources::Text::rtcClassic);
	shootOutString = Resources::getTextByID(Resources::Text::shootOut);
}

void RoundTheClock::serializeConfigCustom(JsonObject& configObject) {
	
}

void RoundTheClock::processConfig(JsonObject& configObject) {

}

void RoundTheClock::drawProperButtons(int index, int id) {
	if (index == 0) {
		rtcButton.guiButton.drawButton(id != 1);
		shootoutButton.guiButton.drawButton(id != 2);
	}
	else if (index == 1) {
		sectorOnlyButton.guiButton.drawButton(id != 1);
		multiplierButton.guiButton.drawButton(id != 2);
		multiplierWithPointButton.guiButton.drawButton(id != 3);
	}	
}

void RoundTheClock::configStart() {
	int buttonWidth = int(SCR_WIDTH * 0.35f);
	int buttonHeight = int(SCR_HEIGHT * 0.166f);	
	
	int buttonStartX = int(SCR_WIDTH * 0.2f);
	int buttonStartY = int(SCR_HEIGHT * 0.375f);

	int buttonOffsetX = int(SCR_WIDTH * 0.6f);
	int buttonOffsetY = int(SCR_HEIGHT * 0.25f);
	
	rtcButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 0 * buttonOffsetY, buttonWidth, buttonHeight, TFT_WHITE, TFT_GREEN, TFT_BLACK, rtcClassicString, 2);
	shootoutButton.setImage(displayContainer->getTFT(), buttonStartX, buttonStartY + 1 * buttonOffsetY, buttonWidth, buttonHeight, TFT_WHITE, TFT_GREEN, TFT_BLACK, shootOutString, 2);

	switch (rtcType) {
		case roundTheClock:
			drawProperButtons(0, 1);
			break;
	
		case shootOut:
			drawProperButtons(0, 2);
			break;

		default:
			break;
	}

	sectorOnlyButton.setImage(displayContainer->getTFT(), buttonStartX + buttonOffsetX, buttonStartY + 0 * buttonOffsetY, buttonWidth, buttonHeight, TFT_WHITE, TFT_GREEN, TFT_BLACK, sectorOnlyString, 2);
	multiplierButton.setImage(displayContainer->getTFT(), buttonStartX + buttonOffsetX, buttonStartY + 1 * buttonOffsetY, buttonWidth, buttonHeight, TFT_WHITE, TFT_GREEN, TFT_BLACK, multiplierString, 2);
	multiplierWithPointButton.setImage(displayContainer->getTFT(), buttonStartX + buttonOffsetX, buttonStartY + 2 * buttonOffsetY, buttonWidth, buttonHeight, TFT_WHITE, TFT_GREEN, TFT_BLACK, multiplierWithPointsString, 2);

	switch (rtcSubType) {
		case sectorOnly:
			drawProperButtons(1, 1);
			break;
	
		case multiplier:
			drawProperButtons(1, 2);
			break;

		case multiplierWithPoint:
			drawProperButtons(1, 3);
			break;
		default:
			break;
	}
}

void RoundTheClock::config(Pair touch) {
	rtcButton.detect(touch);
	shootoutButton.detect(touch);
	sectorOnlyButton.detect(touch);
	multiplierButton.detect(touch);
	multiplierWithPointButton.detect(touch);

	if (rtcButton.simple()) {
		rtcType = roundTheClock;
		drawProperButtons(0, 1);
	}

	if (shootoutButton.simple()) {
		rtcType = shootOut;
		drawProperButtons(0, 2);
	}

	if (sectorOnlyButton.simple()) {
		rtcSubType = sectorOnly;
		drawProperButtons(1, 1);
	}

	if (multiplierButton.simple()) {
		rtcSubType = multiplier;
		drawProperButtons(1, 2);
	}

	if (multiplierWithPointButton.simple()) {
		rtcSubType = multiplierWithPoint;
		drawProperButtons(1, 3);
	}
}

bool RoundTheClock::customStart() {	
	return false;
}

bool RoundTheClock::custom(Pair touch) {

	return false;
}

void RoundTheClock::initializeGame() {
	
}
#include "X01.h"

String X01::getInOutString(int cursor) {
	if (cursor == 1) {
		return doubleString;
	}
	else if (cursor == 2) {
		return masterString;
	}

	return simpleString;
}

void X01::serializeConfigCustom(JsonObject& configObject) {
	
}

void X01::processConfig(JsonObject&) {

}

void X01::configStart() {

	displayContainer->write(SCR_WIDTH / 20, buttonStartY + 0 * buttonOffsetY - buttonHeight / 5, BLACK, 2, (char*) scoreLabel.c_str());

	previousScore.setImage(displayContainer->getTFT(), buttonStartX + SCR_WIDTH / 20, buttonStartY + 0 * buttonOffsetY, buttonWidth / 3, buttonHeight, WHITE, GREEN, BLACK, "<", 2);
	nextScore.setImage(displayContainer->getTFT(), buttonStartX + SCR_WIDTH / 20 + int(SCR_WIDTH * 0.25f), buttonStartY + 0 * buttonOffsetY, buttonWidth / 3, buttonHeight, WHITE, GREEN, BLACK, ">", 2);
	
	int currentScore = scoreMap.getValueByKey(scoreCursor);
	String currentScoreString = String(currentScore);
	if (currentScore < 1000) {
		currentScoreString = " " + currentScoreString;
	}

	displayContainer->writeWithBackground(buttonStartX + SCR_WIDTH / 8, buttonStartY + 0 * buttonOffsetY - buttonHeight / 5, BLACK, CYAN, 2, currentScoreString);

	displayContainer->write(SCR_WIDTH / 20, buttonStartY + 1 * buttonOffsetY - buttonHeight / 5, BLACK, 2, (char*) inLabel.c_str());
	String inString = getInOutString(inCursor);
	inButton.setImage(displayContainer->getTFT(), buttonStartX + SCR_WIDTH / 6, buttonStartY + 1 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) inString.c_str(), 2);

	displayContainer->write(SCR_WIDTH / 20, buttonStartY + 2 * buttonOffsetY - buttonHeight / 5, BLACK, 2, (char*) outLabel.c_str());
	String outString = getInOutString(outCursor);
	outButton.setImage(displayContainer->getTFT(), buttonStartX + SCR_WIDTH / 6, buttonStartY + 2 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) outString.c_str(), 2);

	previousScore.guiButton.drawButton(true);
	nextScore.guiButton.drawButton(true);

	inButton.guiButton.drawButton(true);
	outButton.guiButton.drawButton(true);	
}

void X01::config(Pair touch) {
	previousScore.detect(touch);
	nextScore.detect(touch);

	if (previousScore.simple()) {
		previousScore.guiButton.drawButton(false);

		if (--scoreCursor < 0) {
			scoreCursor = 4;
		}

		int currentScore = scoreMap.getValueByKey(scoreCursor);
		String currentScoreString = String(currentScore);
		if (currentScore < 1000) {
			currentScoreString = " " + currentScoreString;
		}

		displayContainer->writeWithBackground(buttonStartX + SCR_WIDTH / 8, buttonStartY + 0 * buttonOffsetY - buttonHeight / 5, BLACK, CYAN, 2, currentScoreString);
	}

	if (nextScore.simple()) {
		nextScore.guiButton.drawButton(false);

		if (++scoreCursor > 4) {
			scoreCursor = 0;
		}

		int currentScore = scoreMap.getValueByKey(scoreCursor);
		String currentScoreString = String(currentScore);
		if (currentScore < 1000) {
			currentScoreString = " " + currentScoreString;
		}

		displayContainer->writeWithBackground(buttonStartX + SCR_WIDTH / 8, buttonStartY + 0 * buttonOffsetY - buttonHeight / 5, BLACK, CYAN, 2, currentScoreString);
	}

	inButton.detect(touch);
	outButton.detect(touch);

	if (inButton.simple()) {
		inButton.guiButton.drawButton(false);

		if (++inCursor == 3) {
			inCursor = 0;
		}

		String inString = getInOutString(inCursor);
		inButton.setImage(displayContainer->getTFT(), buttonStartX + SCR_WIDTH / 6, buttonStartY + 1 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) inString.c_str(), 2);
	}

	if (outButton.simple()) {
		outButton.guiButton.drawButton(false);

		if (++outCursor == 3) {
			outCursor = 0;
		}

		String outString = getInOutString(outCursor);
		outButton.setImage(displayContainer->getTFT(), buttonStartX + SCR_WIDTH / 6, buttonStartY + 2 * buttonOffsetY, buttonWidth, buttonHeight, WHITE, GREEN, BLACK, (char*) outString.c_str(), 2);
	}

	//visualization
	if (previousScore.released()) {
		previousScore.guiButton.drawButton(true);
	}

	if (nextScore.released()) {
		nextScore.guiButton.drawButton(true);
	}

	if (inButton.released()) {
		inButton.guiButton.drawButton(true);
	}

	if (outButton.released()) {
		outButton.guiButton.drawButton(true);
	}
}

bool X01::customStart() {
	return false;
}

bool X01::custom(Pair touch) {
	return false;
}

void X01::initializeGame() {
	inString = inLabel + " " + getInOutString(inCursor);
	outString = outLabel + " " + getInOutString(outCursor);
}
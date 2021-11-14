#include "AbstractScore.h"
#include "../Player/Player.h"
#include "../Player/PlayerContainer.h"

void AbstractScore::correct(Sector correctable) {
	deleteThrow(correctable);
	status();
	statusAfterHit(correctable);
}

void AbstractScore::status() {
	int textSize = 7;

	Player* currentPlayer = playerContainer->getCurrentPlayer();

	int score = currentPlayer->getScore()->playerScore;
	String point = "P" + String(currentPlayer->getID() + 1) + "-" + String(score);
	
	int x = SCR_WIDTH / 8;
	int y = SCR_WIDTH / 8;

	displayContainer->writeWithBackground(x, y, BLACK, CYAN, textSize, "       ");	
	displayContainer->writeWithBackground(x, y, currentPlayer->getInverseColor(), currentPlayer->getColor(), textSize, point);

	int numberOfPlayers = playerContainer->getNumberOfPlayers();

	if (numberOfPlayers > 1) {
		int s2 = 2;

		int x = int(SCR_WIDTH * 0.8f);
		int y = int(SCR_HEIGHT * 0.25f);

		displayContainer->getTFT()->drawFastHLine(x, y, 2 * s2 * 6 * 3, BLACK);
		Player* nextPlayer;
		
		int playerCursor = playerContainer->getPlayerCursor();

		if (playerCursor + 1 == numberOfPlayers) {
			nextPlayer = playerContainer->getPlayerByNumber(0);
		}
		else {
			nextPlayer = playerContainer->getPlayerByNumber(playerCursor + 1);
		}
		
		int playerScore = nextPlayer->getScore()->getPlayerScore();
		point = "P" + String(nextPlayer->getID() + 1) + ":" + String(playerScore);
		if (playerScore < 10) {
			point += "  ";
		}
		
		if (playerScore < 100 && playerScore > 9){
			point += " ";
		}
		
		displayContainer->writeWithBackground(x, y + int(0.02f * SCR_HEIGHT), BLACK, CYAN, s2, point);
	}
}
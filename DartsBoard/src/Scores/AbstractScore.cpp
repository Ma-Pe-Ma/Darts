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

	/*Player* currentPlayer = PlayerContainer::getCurrentPlayer();

	int score = currentPlayer->getScore()->playerScore;
	String point = "P" + String(currentPlayer->getID() + 1) + "-" + String(score);
	
	int x = SCR_WIDTH / 8;
	int y = SCR_WIDTH / 8;

	DisplayContainer::displayContainer.writeWithBackground(x, y, BLACK, CYAN, textSize, "       ");	
	DisplayContainer::displayContainer.writeWithBackground(x, y, currentPlayer->getInverseColor(), currentPlayer->getColor(), textSize, point);*/






	/*if(Player::number > 1) {
		int s2 = 2;
		DisplayContainer::displayContainer.getTFT()->drawFastHLine(320, 115, 2 * s2 * 6 * 3, BLACK);
		Player* nextPlayer;
		if (Player::cursor + 1 == Player::number) {
			nextPlayer = &Player::players[0];
		}
		else {
			nextPlayer = &Player::players[Player::cursor + 1];
		}
		
		point = "P" + String(nextPlayer->ID + 1) + ":" + String(nextPlayer->score->playerScore);
		if (nextPlayer->score->playerScore < 10) {
			point += "  ";
		}
		
		if (nextPlayer->score->playerScore < 100 && nextPlayer->score->playerScore > 9){
			point += " ";
		}
		
		//if (score[nextPlayer] > 100) {
			//ponti = "P" + String(nextPlayer + 1) + ":" + String(score[nextPlayer]);
		//}
		
		DisplayContainer::displayContainer.writeWithBackground(320, 120, BLACK, CYAN, s2, point);
	}*/
}
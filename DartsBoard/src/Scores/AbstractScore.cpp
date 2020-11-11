#include "AbstractScore.h"
#include "../Player.h"

void AbstractScore::Correct(Sector& correctable) {
	Delete(correctable);
	Status();

	StatusAfterHit(correctable);

	correctable.multiplier = 0;
	correctable.base = 0;
}

void AbstractScore::Status() {
	int s = 7;

	int score = Player::current->score->playerScore;
	String point = "P" + String(Player::current->ID + 1) + "-" + String(score);
	
	int x = 50;
	int y = 50;

	DisplayContainer::displayContainer.WriteWithBackground(x,y, BLACK, CYAN, s, "       ");	
	DisplayContainer::displayContainer.WriteWithBackground(x,y, Player::current->inverseColor, Player::current->color, s, point);	

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
		
		DisplayContainer::displayContainer.WriteWithBackground(320, 120, BLACK, CYAN, s2, point);
	}*/
}
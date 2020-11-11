#include "Player.h"
#include "Scores/AbstractScore.h";
#include "States/GameLogic.h"

#define NR_OF_PLAYERS 8

Player* Player::current;
int Player::cursor;
int Player::number = 1;
const int Player::maxNrOfPlayers = NR_OF_PLAYERS;
Player Player::players[8];

void Player::initializePlayers() {
	for (int i = 0; i < NR_OF_PLAYERS; i++) {
		players[i].ID = i;

		switch(i) {
			case 0:
			players[i].color = YELLOW;
			break;

			case 1:
			players[i].color = MAGENTA;
			break;
	
			case 2:
			players[i].color = BLUE;
			break;

			case 3:
			players[i].color = WHITE;
			break;	

			case 4:
			players[i].color = GREEN;
			break;

			case 5:
			players[i].color = RED;
			break;

			case 6:
			players[i].color = YELLOW / 2;
			break;

			case 7:
			players[i].color = CYAN;
			break;
		}

		players[i].inverseColor = 0xffff - players[i].color;
	}	
}

//http://www.barth-dev.de/online/rgb565-color-picker/
uint16_t Player::convertColor(long RGB888) {
	return (((RGB888&0xf80000)>>8) + ((RGB888&0xfc00)>>5) + ((RGB888&0xf8)>>3));
}

uint16_t Player::convertColor(uint8_t red, uint8_t green, uint8_t blue){
	return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

Player* Player::FindNextPlayer() {
	//change to next non-winning player
	while (true) {
		if(++Player::cursor == Player::number) {
			Player::cursor = 0;
			GameLogic::gameLogic.gamePlayingScreen.roundCounter++;	
		}
		
		if (Player::players[Player::cursor].score->position == -1) {
			Player::current = &Player::players[Player::cursor];
			break;
		}
	}

	return Player::current;
}

void Player::SetCorrectPlayerOrder() {
	Player *newPlayers = new Player[Player::number];

	switch (GameLogic::gameLogic.orderModify) {				
		case cyclical:
			for (int i = 1; i < Player::number; i++) {
				newPlayers[i - 1] = Player::players[i];
			}

			newPlayers[Player::number - 1] = Player::players[0];

			for (int i = 0; i < Player::number; i++) {
				Player::players[i] = newPlayers[i];
				Player::players[i].ID = i;
			}

			break;
			
		case reversed:
			for (int i = 0; i < Player::number; i++) {
				newPlayers[Player::number - 1 - i] = Player::players[i];
			}

			for (int i = 0; i < Player::number; i++) {
				Player::players[i] = newPlayers[i];
				Player::players[i].ID = i;
			}

			break;

		case unchanged:
		
		break;
	}

	delete[] newPlayers;
}
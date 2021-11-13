#include "PlayerContainer.h"

PlayerContainer::PlayerContainer() {

}

void PlayerContainer::init() {
		Serial.println("PC0");

	for (int i = 0; i < NR_OF_PLAYERS; i++) {
		Serial.println("PCX0");
		players[i].setID(i);
		
		Serial.println("PCX0");
		/*switch(i) {
			case 0:
			players[i].setColor(YELLOW);
			break;

			case 1:
			players[i].setColor(MAGENTA);
			break;
	
			case 2:
			players[i].setColor(BLUE);
			break;

			case 3:
			players[i].setColor(WHITE);
			break;	

			case 4:
			players[i].setColor(GREEN);
			break;

			case 5:
			players[i].setColor(RED);
			break;

			case 6:
			players[i].setColor(YELLOW / 2);
			break;

			case 7:
			players[i].setColor(CYAN);
			break;
		}*/

		Serial.println("PCX0");
		players[i].setInverseColor(0xffff - players[i].getColor());
	}	

	Serial.println("PC1");
}

Player* PlayerContainer::findNextPlayer() {
	//change to next non-winning player
	while (true) {
		if(++this->cursor == this->number) {
			this->cursor = 0;
			//GameLogic::gameLogic.gamePlayingScreen.roundCounter++;	
		}
		
		if (this->players[this->cursor].getScore()->getPosition() == -1) {
			this->current = &this->players[this->cursor];
			break;
		}
	}

	return this->current;
}

Player* PlayerContainer::getCurrentPlayer() {
	return this->current;
}

int PlayerContainer::getNumberOfPlayers() {
	return this->number;
}

void PlayerContainer::setNumberOfPlayers(int number) {
	this->number = number;
}

Player* PlayerContainer::getPlayerByNumber(int i) {
	if (i < 0 || i >= maxNrOfPlayers) {
		return nullptr;
	}

	return &this->players[i];
}

int PlayerContainer::getPlayerCursor() {
	return this->cursor;
}

int PlayerContainer::getNumberOfFinishedPlayers() {
	int finishedPlayers = 0;

	for (int i = 0; i < this->number; i++) {
		if (this->players[i].getScore()->getPosition() != -1) {
			finishedPlayers++;
		}
	}

	return finishedPlayers;
}

void PlayerContainer::setCorrectPlayerOrder(OrderModify orderModify) {
	Player *newPlayers = new Player[this->number];

	switch (orderModify) {				
		case cyclical:
			for (int i = 1; i < this->number; i++) {
				newPlayers[i - 1] = this->players[i];
			}

			newPlayers[this->number - 1] = this->players[0];

			for (int i = 0; i < this->number; i++) {
				this->players[i] = newPlayers[i];
				this->players[i].setID(i);
			}

			break;
			
		case reversed:
			for (int i = 0; i < this->number; i++) {
				newPlayers[this->number - 1 - i] = this->players[i];
			}

			for (int i = 0; i < this->number; i++) {
				this->players[i] = newPlayers[i];
				this->players[i].setID(i);
			}

			break;

		case unchanged:
		
		break;
	}

	delete[] newPlayers;

	//set cursor to last person
	this->cursor = this->number - 1;
}
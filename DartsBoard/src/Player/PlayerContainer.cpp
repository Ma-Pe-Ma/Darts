#include "PlayerContainer.h"

PlayerContainer::PlayerContainer() {

}

void PlayerContainer::init() {			
	players[0].setColor(YELLOW);			
	players[1].setColor(MAGENTA);			
	players[2].setColor(BLUE);			
	players[3].setColor(WHITE);			
	players[4].setColor(GREEN);
	players[5].setColor(RED);			
	players[6].setColor(YELLOW / 2);			
	players[7].setColor(CYAN);

	for (int i = 0; i < NR_OF_PLAYERS; i++) {
		players[i].setID(i);
		players[i].setInverseColor(0xffff - players[i].getColor());
	}
}

Player* PlayerContainer::findNextPlayer() {
	//change to next non-winning player
	while (true) {
		if(++this->cursor == this->number) {
			this->cursor = 0;
			(*roundCounter)++;
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
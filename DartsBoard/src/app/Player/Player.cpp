#include "Player.h"
#include "../Scores/AbstractScore.h";
#include "../States/GameLogic.h"

//http://www.barth-dev.de/online/rgb565-color-picker/
uint16_t Player::convertColor(long RGB888) {
	return (((RGB888&0xf80000)>>8) + ((RGB888&0xfc00)>>5) + ((RGB888&0xf8)>>3));
}

uint16_t Player::convertColor(uint8_t red, uint8_t green, uint8_t blue){
	return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}
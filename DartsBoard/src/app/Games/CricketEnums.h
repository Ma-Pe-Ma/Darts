#ifndef CRICKETENUMS_H
#define CRICKETENUMS_H

#define CRICKETTYPESIZE 3
enum CricketType {
	score,
	noscore,
	cutthroat
};

#define CRICKETSETSIZE 3
enum CricketNumberSet {
	classicNumbers,
	allNumbers,
	customNumbers
};

#define CRICKETCUSTOMSIZE 3
enum CricketCustomSet {
	interval,
	randomInterval,
	chaotic
};

#endif
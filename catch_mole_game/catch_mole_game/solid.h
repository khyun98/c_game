#pragma once
#include <time.h>

typedef struct _Hammer {
	int printX, printY;
	int centerX, centerY;
	int posX, posY;
	char feature[4][6];
	int isIng;
	clock_t coolTime;
	clock_t ingTime;
}Hammer;

typedef enum _MoleStatus {
	BURROW = 0, PEEK1,PEEK2, UNBURROW,SMASHED
}MoleStatus;

typedef struct _Mole {
	char feature[3][4];
	MoleStatus moleStatus;
	clock_t movingTime;
	int printX, printY;
	int centerX, centerY;
	int posX, posY;
	clock_t wakeTime;
	int moleRandom;
}Mole;


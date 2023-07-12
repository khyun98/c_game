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
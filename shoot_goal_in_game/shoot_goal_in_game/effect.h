#pragma once

#include <time.h>

typedef struct _EFFECT {
	clock_t StartTime;
	clock_t StayTime;
}EFFECT;

void goalMessage(int x, int y);
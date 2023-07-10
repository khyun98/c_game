#pragma once
#include <time.h>
typedef enum _GAME_STATE {
	INIT = 0, READY, RUNNING, STOP,
	SUCCESS, FAILED, RESULT
}GAME_STATE;

typedef struct _STAGE_INFO {
	int nGoalBall;
	clock_t LimitTime;
	int nGoalDaeLength;
	int nGoalDaeX;
	int nGoalDaeY;
	clock_t GoalDaeMoveTime;
	int nGoalDaeDist;
}STAGE_INFO;
#pragma once
#include <time.h>
typedef struct _PLAYER {
	int nCenterX, nCenterY;//중심좌표
	int nPosX, nPosY;//실제위치좌표
	int nX, nY;//캐릭터 출력 시작 좌표
}PLAYER;

typedef struct _BALL {
	int nIsReady;//공 상태 준비상태1 슛상태0
	int nPosX, nPosY;//따로 중심좌표 필요 X
	clock_t MoveTime;
	clock_t OldTime;
}BALL;
#pragma once
#include <time.h>
typedef struct _PLAYER {
	int nCenterX, nCenterY;//�߽���ǥ
	int nPosX, nPosY;//������ġ��ǥ
	int nX, nY;//ĳ���� ��� ���� ��ǥ
}PLAYER;

typedef struct _BALL {
	int nIsReady;//�� ���� �غ����1 ������0
	int nPosX, nPosY;//���� �߽���ǥ �ʿ� X
	clock_t MoveTime;
	clock_t OldTime;
}BALL;
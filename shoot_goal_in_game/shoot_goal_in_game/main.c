#include <stdio.h>
#include <conio.h>
#include "solid.h"
#include <string.h>
#include "Screen.h"

int g_nScore=0;

PLAYER g_sPlayer;
char g_strPlayer[] = "┗━●━┛";
int g_nLength;

BALL g_sBall;
char g_strBall[] = "0";

GOALDAE g_sGoalDae;

void Init() {
	g_nLength = strlen(g_strPlayer);
	g_sPlayer.nCenterX = (g_nLength+1)/2;
	g_sPlayer.nCenterY = 0;
	g_sPlayer.nPosX = 20;
	g_sPlayer.nPosY = 22;
	g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;

	g_sBall.nIsReady = 1;
	g_sBall.nPosX = g_sPlayer.nPosX;
	g_sBall.nPosY = g_sPlayer.nPosY-1;
	g_sBall.MoveTime = 50;

	g_sGoalDae.nPosX = 20;
	g_sGoalDae.nPosY = 4;
	g_sGoalDae.nLength = 1;
	g_sGoalDae.moveTime = 100;
	g_sGoalDae.nMovement = 1;
	int nGoalDaeLength = g_sGoalDae.nLength * 2 + 1;
	for (int i = 0; i < nGoalDaeLength; i++) {
		g_sGoalDae.nLineX[i] = g_sGoalDae.nPosX + 2 * (i + 1);
		//곱하기 2는 골대 표현 문자가 2바이트를 쓰는 특수문자이기 때문
	}

}

void Update() {//player와 상관없이 독립적으로 게임 자체적으로 업데이트할 내용
	clock_t curTime = clock();//시간간격 기준 동기화 기법

	//골대 데이터 업데이트
	int nGoalDaeLength = g_sGoalDae.nLength * 2 + 1;
	if (curTime - g_sGoalDae.oldTime > g_sGoalDae.moveTime) {
		if (g_sGoalDae.nPosX <= 0|| g_sGoalDae.nPosX + 2 + g_sGoalDae.nLength * 2 + 1 + 2 >= 79) {
			g_sGoalDae.nMovement = g_sGoalDae.nMovement * -1;
		}

		g_sGoalDae.nPosX = g_sGoalDae.nPosX + g_sGoalDae.nMovement;
		for (int i = 0; i < nGoalDaeLength; i++) {
			g_sGoalDae.nLineX[i]= g_sGoalDae.nPosX + 2 * (i + 1);
		}
		g_sGoalDae.oldTime = curTime;
	}

	//공 데이터 업데이트
	if (g_sBall.nIsReady == 1) {
		g_sBall.nPosX = g_sPlayer.nPosX;
		g_sBall.nPosY = g_sPlayer.nPosY-1;
	}
	else {
		if (curTime - g_sBall.OldTime > g_sBall.MoveTime) {//시간간격 기준 동기화 기법
			if (g_sBall.nPosY-1>0) {//경계 충돌 판정
				g_sBall.nPosY--;
				g_sBall.OldTime = curTime;// 'k'키 누르면 oldTime 시간 기록함.
			}
			else {
				g_sBall.nIsReady = 1;
				g_sBall.nPosX = g_sPlayer.nPosX;
				g_sBall.nPosY = g_sPlayer.nPosY - 1;
			}
		}

		//골대와 경계 충돌 판정
		if (g_sBall.nPosY == g_sGoalDae.nPosY) {
			if (g_sGoalDae.nPosX <= g_sBall.nPosX && g_sGoalDae.nLineX[nGoalDaeLength - 1] + 2 + 2 >= g_sBall.nPosX) {
				g_sBall.nIsReady = 1;
				g_sBall.nPosX = g_sPlayer.nPosX;
				g_sBall.nPosY = g_sPlayer.nPosY - 1;
				if (g_sGoalDae.nPosX+2 <= g_sBall.nPosX && g_sGoalDae.nLineX[nGoalDaeLength - 1] + 2 +2 -2 >= g_sBall.nPosX) {
					g_nScore++;
					//골인 판정
				}
			}
		}
	}
}

void Render() {
	char tempBuffer[100] = { 0, };
	ScreenClear();
	
	//골대 렌더
	int nGoalDaeLen=g_sGoalDae.nLength*2+1;
	ScreenPrint(g_sGoalDae.nPosX, g_sGoalDae.nPosY, "□");
	for (int i = 0; i < nGoalDaeLen; i++)
		ScreenPrint(g_sGoalDae.nLineX[i], g_sGoalDae.nPosY, "━");
	ScreenPrint(g_sGoalDae.nLineX[nGoalDaeLen - 1]/*골라인 마지막 좌표*/ + 2, g_sGoalDae.nPosY, "□");


	//플레이어 위치별 렌더
	if (g_sPlayer.nX < 0) {
		ScreenPrint(0, g_sPlayer.nPosY, &g_strPlayer[-1 * g_sPlayer.nX]);
	}
	else if (g_sPlayer.nX + g_nLength > 79) {
		strncpy_s(tempBuffer, sizeof(tempBuffer), g_strPlayer, g_nLength - (g_sPlayer.nX + g_nLength - 79));
		ScreenPrint(g_sPlayer.nX, g_sPlayer.nPosY, tempBuffer);
	}
	else if(g_sPlayer.nX>=0&&g_sPlayer.nX<=79){
		ScreenPrint(g_sPlayer.nX,g_sPlayer.nPosY,g_strPlayer);
	}

	//공 렌더
	ScreenPrint(g_sBall.nPosX, g_sBall.nPosY, g_strBall);
	
	//플레이어 위치 문자 출력
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer),"player position x:%d y:%d", g_sPlayer.nPosX, g_sPlayer.nPosY);
	ScreenPrint(0, 0, tempBuffer);
	
	//공 위치 문자 출력
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer), "ball position x:%d y:%d", g_sBall.nPosX, g_sBall.nPosY);
	ScreenPrint(0, 50, tempBuffer);

	//골대 위치 문자 출력
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer), "ball position x:%d y:%d\n", g_sGoalDae.nPosX + 2 + g_sGoalDae.nLength * 2 + 1 + 2,g_sGoalDae.nPosY);
	ScreenPrint(0, 150, tempBuffer);

	//스코어 위치 문자 출력
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer), "goal score x:%d\n", g_nScore);
	ScreenPrint(0, 200, tempBuffer);

	ScreenFlipping();
}

void Release() {}

int main() {
	int nKey, nRemain;

	ScreenInit();
	Init();

	while (1) {
		if (_kbhit()) {//키보드
			nKey = _getch();
			if (nKey == 'q')
				break;
			switch (nKey) {
			case 'j':
				if (g_sPlayer.nPosX <= 1)
					break;//왼쪽 경계 충돌

				g_sPlayer.nPosX--;//이동
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				if (g_sPlayer.nPosX + nRemain > 79 || g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//좌접
					g_sPlayer.nPosX--;
				//특수기호는 2바이트이므로 1바이트만 잘릴 수 없다.

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'l':
				if (g_sPlayer.nPosX >= 77)
					break;//오른쪽 경계 충돌

				g_sPlayer.nPosX++;//이동
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				//중심좌표로 부터 캐릭터 크기. 우측경계접촉 확인용
				if (g_sPlayer.nPosX + nRemain > 79|| g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//우접
					g_sPlayer.nPosX++;//1바이트 더 옮겨줌

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'k'://슛 동작
				if (g_sBall.nIsReady==1) {
					//g_sBall.nPosX = g_sPlayer.nPosX;
					//g_sBall.nPosY = g_sPlayer.nPosY - 1;
					g_sBall.OldTime = clock();
					g_sBall.nIsReady = 0;
				}
				break;
			}
		}
		Update();
		Render();
	}


	Release();
	ScreenRelease();
	return 0;
}
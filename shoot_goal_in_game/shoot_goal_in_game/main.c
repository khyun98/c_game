#include <stdio.h>
#include <conio.h>
#include "solid.h"
#include <string.h>
#include "Screen.h"
#include "effect.h"
#include "gamestate.h"
//fix 
// state message
// render remain problem
// 
//미션 실패시 키보드 입력 이상함

#define STAGECOUNT 10

int g_nWill = 1;

GAME_STATE g_GAMESTATE;
int g_nStageNum, g_nStageGoal, g_nGameStartTime;

char g_strStateMessage[100];
clock_t g_nOldTime;

STAGE_INFO g_sStageInfo[STAGECOUNT];

EFFECT g_sEffect;
int g_nIsGoal;

int g_nScore=0;

PLAYER g_sPlayer;
char g_strPlayer[] = "┗━●━┛";
int g_nLength;

BALL g_sBall;
char g_strBall[] = "0";

GOALDAE g_sGoalDae;

void Init() {
	//게임 상태 초기화
	g_GAMESTATE = INIT;
	g_nOldTime = clock();//게임 진행용 시간 기록
	g_nStageNum = 0;

	//플레이어 초기화
	g_nLength = strlen(g_strPlayer);
	g_sPlayer.nCenterX = (g_nLength+1)/2;
	g_sPlayer.nCenterY = 0;
	g_sPlayer.nPosX = 20;
	g_sPlayer.nPosY = 22;
	g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;

	//공 초기화
	g_sBall.nIsReady = 1;
	g_sBall.nPosX = g_sPlayer.nPosX;
	g_sBall.nPosY = g_sPlayer.nPosY-1;
	g_sBall.MoveTime = 50;

	//골대 초기화는 업데이트 함수로.

	//골 이펙트 초기화
	g_sEffect.StayTime=3000;
	g_nIsGoal = 0;
}

void UpdatingGame();

void Update() {//player와 상관없이 독립적으로 게임 자체적으로 업데이트할 내용
	clock_t curTime = clock();//시간간격 기준 동기화 기법

	switch (g_GAMESTATE) {
	case INIT:
		if(g_nStageNum==0){
			sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "setting,,, initializing");
			for (int i = 0; i < STAGECOUNT; i++) {
				g_sStageInfo[i].nGoalDaeX = 20 + i*3;
				g_sStageInfo[i].nGoalDaeY = 6- STAGECOUNT / (STAGECOUNT / 3);
				g_sStageInfo[i].nGoalDaeLength = STAGECOUNT/ (STAGECOUNT / 3);
				g_sStageInfo[i].nGoalDaeDist = 69 + i;
				g_sStageInfo[i].nGoalBall = 5 + i *2;
				g_sStageInfo[i].LimitTime = 30000 - i;
				g_sStageInfo[i].GoalDaeMoveTime = 110 - i;
			}
			if (curTime - g_nOldTime > 3000) {
				g_nOldTime = curTime;
				g_nStageNum = 1;
			}
		}
		else {
			sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "stage %d start in 3sec,,,",g_nStageNum);
			if (curTime - g_nOldTime > 3000) {
				g_nOldTime = curTime;
				g_GAMESTATE = READY;
			}
		}
		break;
	case  READY:
		g_nScore = 0;

		//골대 초기화
		g_sGoalDae.nPosX = g_sStageInfo[g_nStageNum - 1].nGoalDaeX;
		g_sGoalDae.nPosY = g_sStageInfo[g_nStageNum - 1].nGoalDaeY;
		g_sGoalDae.nLength = g_sStageInfo[g_nStageNum - 1].nGoalDaeLength;
		g_sGoalDae.moveTime = g_sStageInfo[g_nStageNum - 1].GoalDaeMoveTime;
		g_sGoalDae.nMovement = 1;
		int nGoalDaeLength = g_sGoalDae.nLength * 2 + 1;
		for (int i = 0; i < nGoalDaeLength; i++) {
			g_sGoalDae.nLineX[i] = g_sGoalDae.nPosX + 2 * (i + 1);
			//곱하기 2는 골대 표현 문자가 2바이트를 쓰는 특수문자이기 때문
		}

		g_nStageGoal = g_sStageInfo[g_nStageNum - 1].nGoalBall;

		sprintf_s(g_strStateMessage,sizeof(g_strStateMessage), "stage %d mssion %d goal in %d seconds", g_nStageNum,
			g_nStageGoal,g_sStageInfo[g_nStageNum-1].LimitTime/1000);
		if (curTime - g_nOldTime > 3000) {
			g_GAMESTATE = RUNNING;
			g_nOldTime = curTime;
			g_nGameStartTime = curTime;
		}
		break;
	case RUNNING:
		if (curTime - g_nGameStartTime > g_sStageInfo[g_nStageNum - 1].LimitTime) {
			g_GAMESTATE = STOP;
		}
		else {
			sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "%d seconds left %d goal left",
				(g_sStageInfo[g_nStageNum - 1].LimitTime - (curTime - g_nGameStartTime))/1000,
				g_nStageGoal-g_nScore);
			UpdatingGame();
			if (g_nScore >= g_nStageGoal)
				g_GAMESTATE = STOP;
		}
		break;
	case STOP:
		if (g_nScore >= g_nStageGoal) {
			sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "congrats!");
			if (curTime - g_nOldTime > 3000) {
				g_GAMESTATE = SUCCESS;
				g_nOldTime = curTime;
			}
		}
		else {
			sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "boo");
			if (curTime - g_nOldTime > 3000) {
				g_GAMESTATE = FAILED;
				g_nOldTime = curTime;
			}
		}
		break;
	case SUCCESS:
		if (g_nStageNum >= STAGECOUNT) {
			sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "master wait pls");
			if (curTime - g_nOldTime > 3000) {
				g_GAMESTATE =RESULT;
				g_nOldTime = curTime;
			}
		}
		else {
			sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "next Stage in 3 sec,,,");
			if (curTime - g_nOldTime > 3000) {
				g_GAMESTATE = INIT;
				++g_nStageNum;
				g_nOldTime = curTime;
			}
		}
		break;
	case FAILED:
		sprintf_s(g_strStateMessage,sizeof(g_strStateMessage) ,"try again? y/n");
		break;
	case RESULT:
		sprintf_s(g_strStateMessage, sizeof(g_strStateMessage), "good bye");
		if(curTime-g_nOldTime>3000)
			g_nWill = 0;
		break;
	}
}

void UpdatingGame() {
	clock_t curTime = clock();//시간간격 기준 동기화 기법

	//골대 데이터 업데이트
	int nGoalDaeLength = g_sGoalDae.nLength * 2 + 1;
	if (curTime - g_sGoalDae.oldTime > g_sGoalDae.moveTime) {
		if (g_sGoalDae.nPosX <= 0 || g_sGoalDae.nPosX + 2 + g_sGoalDae.nLength * 2 + 1 + 2 >= g_sStageInfo[g_nStageNum-1].nGoalDaeDist) {
			g_sGoalDae.nMovement = g_sGoalDae.nMovement * -1;
		}

		g_sGoalDae.nPosX = g_sGoalDae.nPosX + g_sGoalDae.nMovement;
		for (int i = 0; i < nGoalDaeLength; i++) {
			g_sGoalDae.nLineX[i] = g_sGoalDae.nPosX + 2 * (i + 1);
		}
		g_sGoalDae.oldTime = curTime;
	}

	//공 데이터 업데이트
	if (g_sBall.nIsReady == 1) {
		g_sBall.nPosX = g_sPlayer.nPosX;
		g_sBall.nPosY = g_sPlayer.nPosY - 1;
	}
	else {
		if (curTime - g_sBall.OldTime > g_sBall.MoveTime) {//시간간격 기준 동기화 기법
			if (g_sBall.nPosY - 1 > 0) {//경계 충돌 판정
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
				if (g_sGoalDae.nPosX + 2 <= g_sBall.nPosX && g_sGoalDae.nLineX[nGoalDaeLength - 1] + 2 + 2 - 2 >= g_sBall.nPosX) {
					g_nScore++;
					g_nIsGoal = 1;
					g_sEffect.StartTime = clock();
					//골인 판정
				}
			}
		}
	}

	//골 세러머니 데이터 업데이트
	if (g_nIsGoal == 1) {
		if (curTime - g_sEffect.StartTime > g_sEffect.StayTime)
			g_nIsGoal = 0;
	}
}

void Render() {
	char tempBuffer[100] = { 0, };
	ScreenClear();
	
	if (g_GAMESTATE == RUNNING) {
		//골대 렌더
		int nGoalDaeLen = g_sGoalDae.nLength * 2 + 1;
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
		else if (g_sPlayer.nX >= 0 && g_sPlayer.nX <= 79) {
			ScreenPrint(g_sPlayer.nX, g_sPlayer.nPosY, g_strPlayer);
		}

		//공 렌더
		ScreenPrint(g_sBall.nPosX, g_sBall.nPosY, g_strBall);

		//플레이어 위치 문자 출력
		//memset(tempBuffer, 0, sizeof(tempBuffer));
		//sprintf_s(tempBuffer, sizeof(tempBuffer),"player position x:%d y:%d", g_sPlayer.nPosX, g_sPlayer.nPosY);
		//ScreenPrint(0, 0, tempBuffer);

		//공 위치 문자 출력
		//memset(tempBuffer, 0, sizeof(tempBuffer));
		//sprintf_s(tempBuffer, sizeof(tempBuffer), "ball position x:%d y:%d", g_sBall.nPosX, g_sBall.nPosY);
		//ScreenPrint(0, 50, tempBuffer);

		//골대 위치 문자 출력
		memset(tempBuffer, 0, sizeof(tempBuffer));
		sprintf_s(tempBuffer, sizeof(tempBuffer), "goaldae position x:%d y:%d\n", g_sGoalDae.nPosX + 2 + g_sGoalDae.nLength * 2 + 1 + 2, g_sGoalDae.nPosY);
		ScreenPrint(0, 1, tempBuffer);

		//스코어 위치 문자 출력
		//memset(tempBuffer, 0, sizeof(tempBuffer));
		//sprintf_s(tempBuffer, sizeof(tempBuffer), "goal score :%d ", g_nScore);
		//ScreenPrint(0, 200, tempBuffer);

		//골 세레머니 출력
		if (g_nIsGoal == 1)
			goalMessage(10, 5);
	}
	

	//게임 상태 문구 출력
	ScreenPrint(0, 0, g_strStateMessage);

	ScreenFlipping();
}

void Release() {}

int main() {
	int g_nKey;
	int nRemain;

	ScreenInit();
	Init();

	while (g_nWill) {
		if (_kbhit()) {//키보드
			g_nKey = _getch();
			if (g_nKey == 'q')
				break;
			switch (g_nKey) {
			case 'y': 
				if (g_GAMESTATE == FAILED) {
					g_GAMESTATE = INIT;
					g_nOldTime = clock();
				}
				break;
			case 'n':
				if (g_GAMESTATE == FAILED) {
					g_GAMESTATE = RESULT;
					g_nOldTime = clock();
				}
				break;
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
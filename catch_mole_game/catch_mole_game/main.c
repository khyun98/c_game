//프로젝트 속성 sdk와 플랫폼 도구 집합 속성 변경함. 10.0->10.19~   142(설정 안됨)->141
#include "Screen.h"
#include "solid.h"
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>


void Init();
void Update();
void Render();
void Release();

clock_t g_cGameTime;
char g_nKey;
char g_strGameMessage[100];
int g_nScore;
int g_nMoleIndex;

Mole g_sMole[9];
void MoleInit(int index);
void MoleBurrow(int index);
void MolePeek1(int index);
void MolePeek2(int index);
void MoleUnBurrow(int index);
void MoleSmashed(int index);

void HammerMoleEvaluate();

Hammer g_sHammer;
void HammerInit();
void HammerIng();
void HammerReady();

int main() {
	ScreenInit();
	Init();

	while (1) {

		if (_kbhit()) {
			g_nKey = _getch();
			if (g_nKey == 'q')
				break;
			switch (g_nKey) {
			case '7':case '8':case '9':// 5,10  25,10  45,10
			case '4':case '5':case '6':// 5,15  25,15  45,15
			case '1':case '2':case '3':// 5,20  25,20  45,20
				//간격 가로 5부터 20씩, 세로 10부터 5씩
				if (g_sHammer.isIng==0) {
					HammerIng();
					HammerMoleEvaluate();
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

void Init() {
	g_cGameTime = clock();

	//Hammer initialize
	HammerInit();

	//Mole initailize
	for(int i=0;i<9;i++)
		MoleInit(i);


}


void Update() {
	clock_t curTime = clock();

	sprintf_s(g_strGameMessage, sizeof(g_strGameMessage),
		"time passed : %d  score : %d\n", (curTime - g_cGameTime)/1000,g_nScore);

	//Hammer ing data update
	if (g_sHammer.isIng == 1) {
		if (curTime - g_sHammer.ingTime > g_sHammer.coolTime) {
			HammerReady();
		}
	}

	//Mole data update
	for (int i = 0; i < 9; i++) {
		if (g_sMole[i].moleStatus == BURROW) {
			MoleBurrow(i);
			if ((curTime - g_cGameTime) % g_sMole[i].moleRandom < 20) {
				g_sMole[i].wakeTime = clock();
				g_sMole[i].moleStatus = PEEK1;
			}
		}
		else if (g_sMole[i].moleStatus == PEEK1) {
			MolePeek1(i);
			if (curTime - g_sMole[i].wakeTime > g_sMole[i].movingTime) {
				g_sMole[i].moleStatus = UNBURROW;
			}
		}
		else if (g_sMole[i].moleStatus == UNBURROW) {
			MoleUnBurrow(i);
			if (curTime - g_sMole[i].wakeTime > g_sMole[i].movingTime * 2) {
				g_sMole[i].moleStatus = PEEK2;
			}
		}
		else if (g_sMole[i].moleStatus == PEEK2) {
			MolePeek2(i);
			if (curTime - g_sMole[i].wakeTime > g_sMole[i].movingTime * 3) {
				g_sMole[i].moleStatus = BURROW;
			}
		}
		else if (g_sMole[i].moleStatus == SMASHED) {
			MoleSmashed(i);
			if (curTime - g_sMole[i].wakeTime > 1500) {
				g_sMole[i].moleStatus = BURROW;
			}
		}
	}
	

	
}

void Render() {
	ScreenClear();

	//temporary
	ScreenPrint(0, 0, g_strGameMessage);

	//mole render
	ScreenPrint(5, 10-2, g_sMole[7 - 1].feature[0]);ScreenPrint(25, 10-2, g_sMole[8 - 1].feature[0]);ScreenPrint(45, 10-2, g_sMole[9 - 1].feature[0]);
	ScreenPrint(5, 10-1, g_sMole[7 - 1].feature[1]);ScreenPrint(25, 10-1, g_sMole[8 - 1].feature[1]);ScreenPrint(45, 10-1, g_sMole[9 - 1].feature[1]);
	ScreenPrint(5, 10, g_sMole[7 - 1].feature[2]);ScreenPrint(25, 10, g_sMole[8 - 1].feature[2]);ScreenPrint(45, 10, g_sMole[9 - 1].feature[2]);
	ScreenPrint(5, 15-2, g_sMole[4 - 1].feature[0]);ScreenPrint(25, 15-2, g_sMole[5 - 1].feature[0]); ScreenPrint(45, 15-2, g_sMole[6 - 1].feature[0]);
	ScreenPrint(5, 15-1, g_sMole[4 - 1].feature[1]);ScreenPrint(25, 15-1, g_sMole[5 - 1].feature[1]); ScreenPrint(45, 15-1, g_sMole[6 - 1].feature[1]);
	ScreenPrint(5, 15, g_sMole[4 - 1].feature[2]);ScreenPrint(25, 15, g_sMole[5 - 1].feature[2]); ScreenPrint(45, 15, g_sMole[6 - 1].feature[2]);										
	ScreenPrint(5,20-2, g_sMole[1 - 1].feature[0]);ScreenPrint(25, 20-2, g_sMole[2 - 1].feature[0]);ScreenPrint(45, 20-2, g_sMole[3 - 1].feature[0]);
	ScreenPrint(5,20-1, g_sMole[1 - 1].feature[1]);ScreenPrint(25, 20-1, g_sMole[2 - 1].feature[1]);ScreenPrint(45, 20-1, g_sMole[3 - 1].feature[1]);
	ScreenPrint(5,20, g_sMole[1 - 1].feature[2]);ScreenPrint(25, 20, g_sMole[2 - 1].feature[2]);ScreenPrint(45, 20, g_sMole[3 - 1].feature[2]);

	//hammer render
	ScreenPrint(g_sHammer.printX, g_sHammer.printY, g_sHammer.feature[0]);
	ScreenPrint(g_sHammer.printX, g_sHammer.printY + 1, g_sHammer.feature[1]);
	ScreenPrint(g_sHammer.printX, g_sHammer.printY + 2, g_sHammer.feature[2]);
	ScreenPrint(g_sHammer.printX, g_sHammer.printY + 3, g_sHammer.feature[3]);


	ScreenFlipping();
}

void Release() {
}

void HammerMoleEvaluate() {
	g_nMoleIndex = atoi(&g_nKey) - 1;
	switch (g_sMole[g_nMoleIndex].moleStatus) {
	case BURROW:case SMASHED:
		g_nScore -= 1;
		break;
	case PEEK1:
		g_nScore += 1;
		g_sMole[g_nMoleIndex].moleStatus = SMASHED;
		g_sMole[g_nMoleIndex].wakeTime = clock();
		break;
	case UNBURROW:
		g_nScore += 3;
		g_sMole[g_nMoleIndex].moleStatus = SMASHED;
		g_sMole[g_nMoleIndex].wakeTime = clock();
		break;
	case PEEK2:
		g_nScore += 2;
		g_sMole[g_nMoleIndex].moleStatus = SMASHED;
		g_sMole[g_nMoleIndex].wakeTime = clock();
		break;
	}
}

void HammerReady() {
	g_sHammer.posX = 60;
	g_sHammer.posY = 10;//hammer 중심좌표 실제 위치
	g_sHammer.printX = g_sHammer.posX - g_sHammer.centerX;
	g_sHammer.printY = g_sHammer.posY - g_sHammer.centerY;
	g_sHammer.isIng = 0;
	strcpy_s(g_sHammer.feature[0], sizeof(g_sHammer.feature[0]), "[   ]");
	strcpy_s(g_sHammer.feature[1], sizeof(g_sHammer.feature[0]), " | | ");
	strcpy_s(g_sHammer.feature[2], sizeof(g_sHammer.feature[0]), " | | ");
	strcpy_s(g_sHammer.feature[3], sizeof(g_sHammer.feature[0]), " |_| ");
}

void HammerIng() {
	//case '7':case '8':case '9':// 5,10  25,10  45,10
	//case '4':case '5':case '6':// 5,15  25,15  45,15
	//case '1':case '2':case '3':// 5,20  25,20  45,20
		//간격 가로 5부터 20씩, 세로 10부터 5씩
	g_nMoleIndex = atoi(&g_nKey) - 1;
	int share = g_nMoleIndex / 3;
	int rest = g_nMoleIndex % 3;
	g_sHammer.posX = 5 + (20 * rest);
	g_sHammer.posY = 20 - (5 * share);
	g_sHammer.printX = g_sHammer.posX - g_sHammer.centerX;
	g_sHammer.printY = g_sHammer.posY - g_sHammer.centerY;
	
	g_sHammer.ingTime = clock();
	g_sHammer.isIng = 1;

	strcpy_s(g_sHammer.feature[0], sizeof(g_sHammer.feature[0]), " _  #");
	strcpy_s(g_sHammer.feature[1], sizeof(g_sHammer.feature[0]), "| |_ ");
	strcpy_s(g_sHammer.feature[2], sizeof(g_sHammer.feature[0]), "|  _ ");
	strcpy_s(g_sHammer.feature[3], sizeof(g_sHammer.feature[0]), "|_|  ");
}

void HammerInit() {
	g_sHammer.centerX = 1;
	g_sHammer.centerY = 4;//g_sHammer.feature[2][0]
	g_sHammer.coolTime = 300;
	HammerReady();	
}

void MoleSmashed(int index) {
	strcpy_s(g_sMole[index].feature[0], sizeof(g_sMole[index].feature[0]), "   ");
	strcpy_s(g_sMole[index].feature[1], sizeof(g_sMole[index].feature[0]), "   ");
	strcpy_s(g_sMole[index].feature[2], sizeof(g_sMole[index].feature[0]), "-_-");
}

void MoleBurrow(int index) {
	strcpy_s(g_sMole[index].feature[0],sizeof(g_sMole[index].feature[0]),"   ");
	strcpy_s(g_sMole[index].feature[1],sizeof(g_sMole[index].feature[0]),"   ");
	strcpy_s(g_sMole[index].feature[2],sizeof(g_sMole[index].feature[0]),"0_0");
}

void MolePeek1(int index) {
	strcpy_s(g_sMole[index].feature[0], sizeof(g_sMole[index].feature[0]), "   ");
	strcpy_s(g_sMole[index].feature[1], sizeof(g_sMole[index].feature[0]), "^-^");
	strcpy_s(g_sMole[index].feature[2], sizeof(g_sMole[index].feature[0]), "|_|");
}

void MoleUnBurrow(int index) {
	strcpy_s(g_sMole[index].feature[0], sizeof(g_sMole[index].feature[0]), ">_<");
	strcpy_s(g_sMole[index].feature[1], sizeof(g_sMole[index].feature[0]), "| |");
	strcpy_s(g_sMole[index].feature[2], sizeof(g_sMole[index].feature[0]), "|_|");
}

void MolePeek2(int index) {
	strcpy_s(g_sMole[index].feature[0], sizeof(g_sMole[index].feature[0]), "   ");
	strcpy_s(g_sMole[index].feature[1], sizeof(g_sMole[index].feature[0]), "@-@");
	strcpy_s(g_sMole[index].feature[2], sizeof(g_sMole[index].feature[0]), "|_|");
}

void MoleInit(int index) {
	srand(index);

	MoleBurrow(index);
	int tempRandom=rand() % 10000 + 1;
	while (tempRandom < 3000)
		tempRandom = rand() % 10000 + 1;
	g_sMole[index].moleRandom = tempRandom;

	int share = index / 3;
	int rest = index % 3;

	//난이도 조절 요소
	g_sMole[index].movingTime = 400;

	//고정 요소
	g_sMole[index].posX = 5 + (20 * rest);
	g_sMole[index].posY = 20 - (5 * share);
	g_sMole[index].moleStatus = BURROW;	
	g_sMole[index].centerX=1;
	g_sMole[index].centerY=2;
	g_sMole[index].printX=g_sMole[index].posX-g_sMole[index].centerX;
	g_sMole[index].printY=g_sMole[index].posY-g_sMole[index].centerY;
}
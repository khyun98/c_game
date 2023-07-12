//프로젝트 속성 sdk와 플랫폼 도구 집합 속성 변경함. 10.0->10.19~   142(설정 안됨)->141
#include "Screen.h"
#include "solid.h"
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

void Init();
void Update();
void Render();
void Release();

char g_nKey;

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
	//Hammer initialize
	HammerInit();





}


void Update() {
	clock_t curTime = clock();
	if (g_sHammer.isIng == 1) {
		if (curTime - g_sHammer.ingTime > g_sHammer.coolTime) {
			HammerReady();
		}
	}
}

void Render() {
	ScreenClear();

	//hammer render
	ScreenPrint(g_sHammer.printX, g_sHammer.printY  , g_sHammer.feature[0]);
	ScreenPrint(g_sHammer.printX, g_sHammer.printY+1, g_sHammer.feature[1]);
	ScreenPrint(g_sHammer.printX, g_sHammer.printY+2, g_sHammer.feature[2]);
	ScreenPrint(g_sHammer.printX, g_sHammer.printY+3, g_sHammer.feature[3]);

	//temporary
	ScreenPrint(5, 10, "7");ScreenPrint(25, 10, "8");ScreenPrint(45, 10, "9");
	ScreenPrint(5, 15, "4");ScreenPrint(25, 15, "5");ScreenPrint(45, 15, "6");
	ScreenPrint(5,20, "1");ScreenPrint(25, 20, "2");ScreenPrint(45, 20, "3");


	ScreenFlipping();
}

void Release() {
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
	int num = atoi(&g_nKey) - 1;
	int share = num / 3;
	int rest = num % 3;
	g_sHammer.posX = 5 + (20 * rest);
	g_sHammer.posY = 20 - (5 * share);
	g_sHammer.printX = g_sHammer.posX - g_sHammer.centerX;
	g_sHammer.printY = g_sHammer.posY - g_sHammer.centerY;
	g_sHammer.ingTime = clock();
	g_sHammer.isIng = 1;

	strcpy_s(g_sHammer.feature[0], sizeof(g_sHammer.feature[0]), " _  #");
	strcpy_s(g_sHammer.feature[1], sizeof(g_sHammer.feature[0]), "| |_ ");
	strcpy_s(g_sHammer.feature[2], sizeof(g_sHammer.feature[0]), "|  _|");
	strcpy_s(g_sHammer.feature[3], sizeof(g_sHammer.feature[0]), "|_|  ");
}

void HammerInit() {
	g_sHammer.centerX = 2;
	g_sHammer.centerY = 4;//g_sHammer.feature[2][0]
	g_sHammer.coolTime = 1000;
	HammerReady();	
}
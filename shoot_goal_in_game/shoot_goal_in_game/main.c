#include <stdio.h>
#include <conio.h>
#include "solid.h"
#include <string.h>
#include "Screen.h"

PLAYER g_sPlayer;
char g_strPlayer[] = "┗━●━┛";
int g_nLength;

void Init() {
	g_nLength = strlen(g_strPlayer);
	g_sPlayer.nCenterX = (g_nLength+1)/2;
	g_sPlayer.nCenterY = 0;
	g_sPlayer.nPosX = 20;
	g_sPlayer.nPosY = 22;
	g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
}

void Update() {}

void Render() {
	char tempBuffer[100] = { 0, };
	ScreenClear();
	

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
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer),"player position x:%d y:%d", g_sPlayer.nPosX, g_sPlayer.nPosY);
	ScreenPrint(0, 0, tempBuffer);
	

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
					break;
				g_sPlayer.nPosX--;//이동
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				if (g_sPlayer.nPosX + nRemain > 79 || g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//좌접
					g_sPlayer.nPosX--;
				//특수기호는 2바이트이므로 1바이트만 잘릴 수 없다.

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'l':
				if (g_sPlayer.nPosX >= 77)
					break;
				g_sPlayer.nPosX++;//이동
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				//중심좌표로 부터 캐릭터 크기. 우측경계접촉 확인용
				if (g_sPlayer.nPosX + nRemain > 79|| g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//우접
					g_sPlayer.nPosX++;//1바이트 더 옮겨줌

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'k':
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
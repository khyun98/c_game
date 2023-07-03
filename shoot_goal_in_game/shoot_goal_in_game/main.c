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
	g_sPlayer.nCenterX = 0;
	g_sPlayer.nPosX = 20;
	g_sPlayer.nPosY = 22;
	g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
}

void Update() {}

void Render() {
	
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
				g_sPlayer.nPosX--;//이동
				if (g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//좌접
					g_sPlayer.nPosX--;
				//특수기호는 2바이트이므로 1바이트만 잘릴 수 없다.

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'l':
				g_sPlayer.nPosX++;//이동
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				//중심좌표로 부터 캐릭터 크기. 우측경계접촉 확인용
				if (g_sPlayer.nPosX + nRemain > 79)//우접
					g_sPlayer.nPosX++;//1바이트 더 옮겨줌

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'k':
				break;
			}
		}
	}

	Release();
	ScreenRelease();
	return 0;
}